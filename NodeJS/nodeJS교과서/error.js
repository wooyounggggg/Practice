const fs=require('fs');
setInterval(()=>{
    console.log('시작');
    fs.unlink('./asdad.js',(err)=>{
        if(err){
            console.log('시작');
            console.log(err);
            console.log('끝');
        }
    });
    // try{
    //     throw new Error('서버를 고장내주마');
    // }catch(err){
    //     console.log(err);
    // }
},1000);
