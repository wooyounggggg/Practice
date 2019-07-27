const crypto = require('crypto');

console.log(crypto.createHash('sha512').update('password').digest('base64'));