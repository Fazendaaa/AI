import fs from 'fs';
import {
    Meli
} from 'mercadolibre';
import dotenv from 'dotenv';
import removeAccents from 'remove-accents';

dotenv.config();
const app = new Meli(process.env.CLIENT_ID, process.env.CLIENT_KEY);

/**
 * Filter's the Mercado Libre's info into only those that matter to this application.
 * @param {String} type - Item's type.
 * @param {JSON} element - Mercado Libre's content.
 * @returns {JSON} Element seted with program content requests.
 */
const itemToJSON = (type, element) => {
    return {
        type,
        // Certains items don't have an orignal price, so price must be seted instead.
        original_price: (element.original_price) ? element.original_price : element.price,
        price: element.price,
        sold_quantity: element.sold_quantity,
        // Since many sellers put accents in the brand name, so this not creates another brand remove it.
        brand: (element.attributes && element.attributes[0] && element.attributes[0].value_name) ?
               removeAccents(element.attributes[0].value_name) : 'Sem Marca'
    };
}

/**
 * This function takes the info about given element then returns it string format.
 * @param {JSON} element - Serached element.
 * @returns {String} Element info setted int string format.
 */
const stringfyResponse = element => {
    // Since brand name could have space and Weka doesn't recognize it as string, underscore must be used. And all names
    // with '%' should be removed because it's how weka implements commentary.
    return `${element.original_price}, ${element.price}, ${element.sold_quantity}, \
${element.brand.replace(/ |%/g, '_').replace(/'|%/g, '')}, ${element.type}`;
}

/**
 * Mercado Libre API doesn't allow to retrive all possible values at once, so pagination is needed it.
 * @param {String} query - Item to be searched for.
 * @param {Number} offset - Initial search offset.
 * @returns {Object[JSON]} Info about the searched element.
 */
const paging = (query, offset) => new Promise((resolve, reject) => {
    app.get(`sites/MLB/search?q=${query}&offset=${offset}`, (err, res) => {
        if(err)
            reject(err);
        else 
            Promise.all(res.results.map(element => {
                return itemToJSON(query, element);
            })).then(resolve).catch(reject);
    });
});

/**
 * This  function  seraches  for elements  in  Mercado  Livre webservice and returns info about: original price, current
 * price, sold quantity and brand.
 * @param {String} query - Item to be searched for.
 * @returns {Object[JSON]} Info about the searched element.
 */
const search = query => new Promise((resolve, reject) => {
    // To  change  search  to other countries different then BRAZIL change MLB to Mercado Libre desired country. Mercado
    // Libre's API limits up to 50 000 results.
    app.get(`sites/MLB/search?q=${query}`, (err, res) => {
        if(err)
            reject(err);
        else {
            // 200 is the limit for page results.
            const limit = parseInt(res.paging.total/200);
            const length = (1 < limit) ? limit : 1;
            // Set each start offset
            const offset = Array.apply(null, {length}).map((element, index) => {
                return (index+1)*200;
            });

            Promise.all(offset.map(element => {
                return paging(query, element);
            })).then(result => {
                // Flattening arrary
                return [].concat.apply([], result);
            }).then(resolve).catch(reject);
        }
    });
});

/**
 * Writes in arff file the info wanted.
 */
fs.open('results.arff', 'w', (err, fd) => {
    const fileHeader = `%   ${Date(Date.now())}\n\
%   Banco de dados para suplementos retirados do Mercado Livre\n\
%\n\
%   Creators:\n\
%       Danilo de Moraes Costa\n\
%       Lucas de Almeida Carotta\n\
%\n\
\n\
@relation Sumplementos\n\
\n\
@attribute Preco_original       NUMERIC\n\
@attribute Preco_de_venda       NUMERIC\n\
@attribute Quantidade_vendida   NUMERIC\n\
@attribute Marca                STRING\n\
@attribute Produtos             {whey, bcaa, termogenico, glutamina, creatina}\n\
\n\
@data\n\
`;
    if(err)
        console.log(err);
    else {
        Promise.all([
            search('whey'),
            search('bcaa'),
            search('termogenico'),
            search('glutamina'),
            search('creatina')
        ]).then(result => {
            return [].concat.apply([], result);
        }).then(result => {
            // Transform elements into string response.
            return result.map(stringfyResponse).join('\n');
        }).then(result => {
            fs.write(fd, fileHeader.concat(result), (err, written, str) => {
                if(err)
                    console.log(err);
            });
        }).catch(console.log);
    }
});
