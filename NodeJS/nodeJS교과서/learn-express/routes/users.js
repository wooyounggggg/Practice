var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/', (req, res) => {
  res.send('Hello users');
});

//app.use의 첫번째 인자와 router.post의 첫번째 인자가 합쳐진 string이 url이 된다!!
router.post('/', (req, res) => {

});
module.exports = router;