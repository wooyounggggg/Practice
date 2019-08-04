const http = require('http');
const https = require('https');
const http2 = require('http2');
const fs = require('fs');

http.createServer((req, res) => {
    res.end('http server');
}).listen(80);


//https는 인증서가 필요!
https.createServer({
    cert: fs.readFileSync('도메인 인증서 경로'),
    key: fs.readFileSync('도메인 비밀키 경로'),
    ca: [
        fs.readFileSync('상위 인증서 경로'),
        fs.readFileSync('상위 인증서 경로'),
    ]
}, (req, res) => {
    res.end('https server');
}).listen(443);

//http2는 https를 기반으로 하기 때문에 인증서 필요
http2.createSecureServer({
    cert: fs.readFileSync('도메인 인증서 경로'),
    key: fs.readFileSync('도메인 비밀키 경로'),
    ca: [
        fs.readFileSync('상위 인증서 경로'),
        fs.readFileSync('상위 인증서 경로'),
    ]
}, (req, res) => {

}).listen(443);