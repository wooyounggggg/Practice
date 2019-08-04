const http = require('http');
const fs = require('fs');
const portNum = 8005;
const users = {

};
const router = {
    get: {
        '/': (req, res) => {
            return fs.readFile('./restFront.html', (err, data) => {
                if (err) throw err;
                res.end(data);
            });
        },
        '/users': (req, res) => {
            res.end(JSON.stringify(users));
        },
        '/favicon.ico': (req, res) => {
            console.log('favicon');
        },
        '*': (req, res) => {
            return fs.readFile(`.${req.url}`, (err, data) => {
                if (err) {
                    throw err;
                }
                res.end(data);
            });
        },
        post: {
            '/users': (req, res) => {
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
        },
        put: {
            '/users': (req, res) => {
                const id = req.url.split('/')[2];
                let body = '';
                req.on('data', (chunk) => {
                    body += chunk;
                });
                return req.on('end', () => {
                    console.log('put', body);
                    users[id] = JSON.parse(body).name;
                    return res.end((JSON.stringify(users)));
                });
            }
        },
        patch: {
            '/': (req, res) => {

            },
            '/users': (req, res) => {

            }
        },
        delete: {
            '/users': (req, res) => {
                const id = req.url.split('/')[2];
                let body = '';
                req.on('data', (chunk) => {
                    body += chunk;
                });
                return req.on('end', () => {
                    console.log('delete', body);
                    delete users[id];
                    return res.end(JSON.stringify(users));
                })
            }
        }
    }
};

http.createServer((req, res) => {
    const matchedUrl = router[req.method.toLowerCase()][req.url];
    (matchedUrl || router[req.method.toLowerCase()]['*'])(req, res);
}).listen(portNum, () => {
    console.log(`${portNum}번 포트에서 대기중입니다.`)
});