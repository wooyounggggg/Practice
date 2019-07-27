const queryString = require('querystring');
const url = require('url');

const parsedUrl = url.parse('https://www.naver.com/?page=3');
const query = queryString.parse(parsedUrl.query);
console.log('querystring.parse() :',query);
console.log('querystring.stringify() :',queryString.stringify(query));