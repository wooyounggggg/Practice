const http = require('http');
const fs = require('fs');

const parseCookies = (cookie = '') =>
    cookie
    .split(';')
    .map(v => v.split('='))
    .map(([k, ...vs]) => [k, vs.join('=')])
    .reduce((acc, [k, v]) => {
        acc[k.trim()] = decodeURIComponent(v);
        return acc;
    }, {});

const server = http.createServer((req, res) => {
    res.writeHead(200, {
        'Set-Cookie': 'mycookie=test'
    });
    console.log(req.url, parseCookies(req.headers.cookie));
    console.log('서버 실행');
    fs.readFile('./server.html', (err, data) => {
        if (err) {
            throw err;
        }
        res.end(data);
    });
}).listen(8080);

server.on('listening', () => {
    console.log('hi');
});

server.on('error', (err) => {
    console.error(err);
});