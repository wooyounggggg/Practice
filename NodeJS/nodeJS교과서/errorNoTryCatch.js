process.on('uncaughtException',(err)=>{
    console.error('예기치 못한 에러',err);
});

setInterval(()=>{
    throw new Error('서버 고장');
},1000);
setTimeout(()=>{
    console.log('실행');
},2000);

//error는 기본적으로 모두 잡는다는 마인드로 합시다.