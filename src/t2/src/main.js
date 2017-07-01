import fs from 'fs';
import {
    Meli
} from 'mercadolibre';
import dotenv from 'dotenv';

dotenv.config();
const app = new Meli(process.env.CLIENT_ID, process.env.CLIENT_KEY);

/**
 * This  function  seraches  for elements  in  Mercado  Livre webservice and returns info about: original price, current
 * price, sold quantity and brand.
 * @param {String} query - Item to be searched for.
 * @returns {Object[JSON]} Info about the searched element.
 */
const search = query => new Promise((resolve, reject) => {
    // To change search to other countries different then BRAZIL change MLB to Mercado Libre desired country.
    app.get(`sites/MLB/search?q=${query}`, (err, res) => {
        if(err)
            reject(err);
        else {
            // Since for this application there's no need to search more then 50 items, no pagination it's needed.
            Promise.all(res.results.map((element) => {
                // All price info will be in reais -- Brazil's currency.
                return {
                    // Certains items don't have an orignal price, so price must be seted instead.
                    type: query,
                    original_price: (element.original_price) ? element.original_price : element.price,
                    price: element.price,
                    sold_quantity: element.sold_quantity,
                    brand: (element.attributes[0]) ? element.attributes[0].value_name : 'sem marca'
                };
            })).then(resolve);
        }
    });
});

/**
 * This function takes the info about given element then returns it string format.
 * @param {JSON} element - Serached element.
 * @returns {String} Element info setted int string format.
 */
const stringfyResponse = element => {
    return `${element.original_price}, ${element.price}, ${element.sold_quantity}, ${element.brand}, ${element.type}`;
}

/**
 * Writes in arff file the info wanted.
 */
fs.open('results.arff', 'w', (err, fd) => {
    const fileHeader = "%   Banco de dados para suplementos retirados do Mercado Livre\n\
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
@attribute Marca                NUMERIC\n\
@attribute Produtos             {whey, bcaa, termogenico, glutamina}\n\
\n\
@data\n\
";
    if(err)
        console.log(err);
    else {
        Promise.all([search('whey'), search('bcaa'), search('termogenico'), search('glutamina')]).then(result => {
            // Flattening array.
            return [].concat.apply([], result);
        }).then(result => {
            // Trnasform elements into string response.
            return result.map(stringfyResponse).join('\n');
        }).then(result => {
            fs.write(fd, fileHeader.concat(result), (err, written, str) => {
                if(err)
                    console.log(err);
            });
        }).catch(console.log);
    }
});
