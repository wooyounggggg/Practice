const http = require('http');
const fs = require('fs');
const portNum = 8005;
const users = {

};
http.createServer((req, res) => {
    if (req.method === 'GET') {
        if (req.url === '/') {
            return fs.readFile('./restFront.html', (err, data) => {
                if (err) throw err;
                res.end(data);
            });
        } else if (req.url === '/users') {
            res.end(JSON.stringify(users));
        } else if (req.url === '/favicon.ico') {
            console.log('favicon');
        } else return fs.readFile(`.${req.url}`, (err, data) => {
            if (err) {
                throw err;
            }
            res.end(data);
        });
    } else if (req.method === 'POST') {
        if (req.url === '/') {

        } else if (req.url === '/users') {
            let body = '';
            req.on('data', (chunk) => {
                body += chunk;
            });
            req.on('end', () => {
                console.log('POST 본문(body)', body);
                const {
                    name
                } = JSON.parse(body);
                const id = +new Date();
                users[id] = name;
                res.writeHead(201, {
                    'Content-Type': 'text/html; charset=utf-8'
                });
                res.end('사용자 등록 성공');
            });
        }
    } else if (req.method === 'PUT') {
        if (req.url === '/') {

        } else if (req.url === '/users') {

        }
    } else if (req.method === 'PATCH') {
        if (req.url === '/') {

        } else if (req.url === '/users') {

        }
    } else if (req.method === 'DELETE') {
        if (req.url === '/') {

        } else if (req.url === '/users') {

        }
    }
}).listen(portNum, () => {
    console.log(`${portNum}번 포트에서 대기중입니다.`)
});