const http = require('http');
const fs = require('fs');
const url = require('url');
const qs = require('querystring');
const portNum = 8081;
const parseCookies = (cookie = '') =>
    cookie
    .split(';')
    .map(v => v.split('='))
    .map(([k, ...vs]) => [k, vs.join('=')])
    .reduce((acc, [k, v]) => {
        acc[k.trim()] = decodeURIComponent(v);
        return acc;
    }, {});

const session = {

};

const server = http.createServer((req, res) => {
    console.log(req.url, parseCookies(req.headers.cookie));
    const cookies = parseCookies(req.headers.cookie);
    if (req.url.startsWith('/login')) {
        const {
            query
        } = url.parse(req.url);
        const {
            name
        } = qs.parse(query);
        const expires = new Date();
        const randomInt = +new Date();
        session[randomInt] = {
            name,
            expires,
        };
        expires.setMinutes(expires.getMinutes() + 5);
        res.writeHead(302, {
            Location: '/',
            'Set-Cookie': `session=${randomInt}; name=${encodeURIComponent(name)}; Expires=${expires.toGMTString()}; HttpOnly; Path=/`
        });
        res.end(name);
    } else if (cookies.session && session[cookies.session].expires > new Date()) /*유효기간 check*/ {
        res.writeHead(200, {
            'Content-Type': 'text/html; charset=utf-8'
        });
        res.end(`${session[cookies.session].name}님 안녕하세요!`);
    } else {
        fs.readFile('./server4.html', (err, data) => {
            if (err) {
                throw err;
            }
            res.end(data);
        });
    }
}).listen(portNum);

server.on('listening', () => {
    console.log(`You can enter the server port ${portNum}`)
});

server.on('error', (err) => {
    console.error(err);
});