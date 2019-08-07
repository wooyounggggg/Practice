const express = require('express');
const router = express.Router();

/* GET home page. */
router.use((req, res, next) => { //request가 되기 전에 middle ware를 거쳐감!
  console.log('첫 번째 미들웨어');
  next(); // ※next를 붙여줘야 다음 미들웨어로 넘어가거나 res.send로 response를 전송함!!!
});
router.use((req, res, next) => {
  console.log('두 번째 미들웨어');
  next();
});

router.get('/', (req, res) => { //얘네도 전부다 조건부로 동작하는 라우팅 미들웨어임!!
  console.log('세 번째 미들웨어');
  res.render('test');
});
router.post('/', (req, res) => {

});



module.exports = router;