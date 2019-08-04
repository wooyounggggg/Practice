const http = require('http');
const cluster = require('cluster');
const os = require('os');
const numCpus = os.cpus().length;

if (cluster.isMaster) {
    console.log('Master Process Id', process.pid);
    for (let i = 0; i < numCpus; i++) {
        cluster.fork();
    }
    cluster.on('exit', (worker, code, signal) => {
        console.log(worker.process.pid, 'worker exited!');
    });
} else {
    http.createServer((req, res) => {
        res.end('http server');
        setTimeout(() => {
            process.exit(1)
        }, 1000);
    }).listen(8080);
    console.log(process.pid, 'Worker execute');
}