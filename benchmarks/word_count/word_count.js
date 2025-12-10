// Word count benchmark - JavaScript
// Count words and lines in large generated text (classic MapReduce task)

const n = parseInt(process.argv[2]) || 100000;

const phrase = "the quick brown fox jumps over the lazy dog\n";

// Generate text: repeat phrase n times
const text = phrase.repeat(n);

// Count words and lines
const words = text.split(/\s+/).filter(w => w.length > 0).length;
const lines = (text.match(/\n/g) || []).length;

console.log(`${words} ${lines}`);
