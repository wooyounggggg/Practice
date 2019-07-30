const fs=require('fs');
const readStream = fs.copyFile('./readme4.txt','./writeme4.txt',(err)=>{
    console.log(err);
});
// const readStream=fs.createReadStream('readme4.txt');
// const writeStream=fs.createWriteStream('writeme3.txt');
// readStream.pipe(writeStream);
