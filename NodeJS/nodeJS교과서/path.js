const path = require('path');
console.log(`path.sep : ${path.sep}`);
console.log(`path.delimiter : ${path.delimiter}`);
console.log(`path.dirname : ${path.dirname(__filename)}`);
console.log(`path.extname : ${path.extname(__filename)}`);
console.log(`path.basename : ${path.basename(__filename)}`);
console.log(`path.parse : ${path.parse(__filename,{depth:2})}`);
console.log(`path.format : ${path.format(path.parse(__filename))}`);
console.log(`path.normalize : ${path.normalize('C://users\\\zerocho\\\path.js')}`)
console.log(`path.isAbsolute : ${path.isAbsolute(__filename)}`);
console.log(`path.relative : ${path.relative('C:\\users\\zerocho\\path.js','C:\\')}`);
console.log(`path.join : ${path.join(__dirname,'..','..','/users','.','/zerocho')}`);
console.log(`path.resolve : ${path.resolve(__dirname,'..','..','/users','.','/zerocho')}`)