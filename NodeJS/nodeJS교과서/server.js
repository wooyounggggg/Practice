const http = require('http');
const fs = require('fs');
const portNum = 8080;
const server = http.createServer((req,res)=>{
    console.log('서버 실행');
    fs.readFile('./server.html',(err, data)=>{
        if(err){
            throw err;
        }
        res.end(data);
    });
}).listen(portNum);
server.on('listening',()=>{
    console.log(`You can enter the server port ${portNum}`)
});
server.on('error',(err)=>{
    console.error(err);
})