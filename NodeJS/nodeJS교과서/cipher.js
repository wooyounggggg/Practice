const crypto = require('crypto');
const cipher = crypto.createCipher('aes-256-cbc','열쇠');
let cipherResult = cipher.update('zerocho','utf8','base64'); // utf8 -> base64로 바꿔라
cipherResult += cipher.final('base64');
console.log('암호 :',cipherResult);

const decipher = crypto.createDecipher('aes-256-cbc','열쇠');
let decipherReulst = decipher.update(cipherResult,'base64','utf8'); // base64 -> utf8로 바꿔라
decipherReulst += decipher.final('utf8');
console.log('평문 :',decipherReulst);