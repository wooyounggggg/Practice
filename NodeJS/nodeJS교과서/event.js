const eventEmitter = require('events');
const myEvent = new eventEmitter();

// addListener = on
myEvent.addListener('방문',()=>{
    console.log('방문해주셔서 감사합니다.');
});
myEvent.on('종료',()=>{
    console.log('bye');
});
myEvent.on('종료',()=>{
    console.log('제발 좀 가세요');
});
myEvent.once('특별 이벤트',()=>{
    console.log('한 번만 실행됩니다.');
});
myEvent.emit('방문');
myEvent.emit('종료');
myEvent.on('계속',()=>{
    console.log('계속 리스닝');
});

console.log(myEvent.listenerCount('방문'));
// myEvent.removeAllListeners(); //모든 event lister를 제거.
// myEvent.emit('계속');
