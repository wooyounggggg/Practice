console.time('전체 시간');
for(let i=0;i<100000;i++){
    continue;
}
console.timeEnd('전체 시간');

let a = () => console.trace('에러 위치 추적');
let b = () => a();
let c = () => b();
let d = () => c();

d();

console.dir(console,{colors:true, depth:1});
console.dir(console,{colors:true, depth:2});