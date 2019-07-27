const url = 'https://www.naver.com'
const urlModule = require('url');

const URL = urlModule.URL;
const myURL = new URL(url);

console.log('new URL() :', myURL);
console.log('url.format() :',urlModule.format(myURL));
console.log('---------------------------------')
const parsedUrl = urlModule.parse(url);
console.log('url.parse() :',parsedUrl);