const fs = require('fs');
const writeStream = fs.WriteStream('./writeme2.txt');

writeStream.on('finish',()=>{
    console.log('end');
});
writeStream.write('이 글을 씁니다.\n');
writeStream.write('한 번 더 씁니다.\n');
writeStream.end();
