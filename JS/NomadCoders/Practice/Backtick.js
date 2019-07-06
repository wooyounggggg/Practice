// function sayHello(name, age){
//   return `Hello ${name} you are ${age} years old.`;
// }

// console.log(greetNicolas='abc');

// calculator:object
const calculator={
 plus: function(a,b){return a+b;},
 sub: function(a,b){return a-b;},
 mul:function(a,b){return a*b;},
 div:function(a,b){return a/b;},
 power:function(a,b){return a**b;},
 root:function(a){return Math.sqrt(a);}
}

console.log(calculator.root(5));