const n = parseInt(process.argv[2]) || 100000;

let result = "";
for (let i = 0; i < n; i++) {
    result += "a";
}

console.log(result.length);
