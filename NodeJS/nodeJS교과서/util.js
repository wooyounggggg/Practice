const util = require('util');
const crypto = require('crypto');

const dontUseMe = util.deprecate((x,y) => {
    console.log(x+y);
},'This method will be deprecated in 2019.10');

// dontUseMe(1,2);

//randomBytes - no Promise!
crypto.randomBytes(64,(err,buf) => {
    const salt = buf.toString('base64');
    crypto.pbkdf2('zerocho',salt, 1000000, 64, 'sha512', (err,key) => {
        console.log('password',key.toString('base64'));
        console.log('\n');
    })
});

// promisified randomBytes
const randomBytesPromise = util.promisify(crypto.randomBytes);
const pbkdf2Promise = util.promisify(crypto.pbkdf2);

randomBytesPromise(64)
    .then((buf) => {
        const salt = buf.toString('base64');
        return pbkdf2Promise('zerocho',salt,100000,64,'sha512');
    })
    .then((key) => {
        console.log('password',key.toString('base64'));
        console.log('\n');
    })
    .catch((err) => {  
        console.error(err);
    });

(async () => {
    const buf=await randomBytesPromise(64);
    const salt = buf.toString('base64');
    const key = await pbkdf2Promise('zerocho',salt,1000000,64,'sha512');
    console.log('password',key.toString('base64'));
})();