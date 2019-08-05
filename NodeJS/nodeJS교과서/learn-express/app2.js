const express = require('express');
const app = express();

app.get('/', (req, res) => {
    res.send('Hello Express!');
});

app.get('/users', (req, res) => {
    res.send('Hello users');
});


module.exports = app;