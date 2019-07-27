const crypto = require('crypto');

crypto.randomBytes(64,(err, buf) => {
    const salt = buf.toString('base64');
    console.log('\nsalt :',salt);
    console.time('암호화');
    crypto.pbkdf2('zero', salt, 1000000, 64, 'sha512', (err,key) =>{
        console.log('\npassword :',key.toString('base64'));
        console.timeEnd('암호화');
    });
});