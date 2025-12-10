const iterations = parseInt(process.argv[2]) || 10000;

// Generate test text with patterns to match
const base = `The quick brown fox jumps over the lazy dog. \
Email: user123@example.com Phone: 555-1234 \
Date: 2024-01-15 Price: $99.99 \
URL: https://www.example.com/path?query=value \
IP: 192.168.1.1 Code: ABC-123-XYZ
`;

const text = base.repeat(iterations);

// Various regex patterns to test different matching scenarios
const patterns = [
    /[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}/g,  // Email
    /[0-9]{3}-[0-9]{4}/g,                                // Phone
    /[0-9]{4}-[0-9]{2}-[0-9]{2}/g,                       // Date
    /\$[0-9]+\.[0-9]{2}/g,                               // Price
    /https?:\/\/[a-zA-Z0-9.-]+\/[a-zA-Z0-9/?=&._-]*/g,   // URL
    /[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}/g,   // IP
    /[A-Z]{3}-[0-9]{3}-[A-Z]{3}/g,                       // Code
    /\b[a-z]{4,}\b/g                                     // 4+ letter words
];

let totalMatches = 0;
for (const pattern of patterns) {
    const matches = text.match(pattern);
    if (matches) {
        totalMatches += matches.length;
    }
}

console.log(totalMatches);
