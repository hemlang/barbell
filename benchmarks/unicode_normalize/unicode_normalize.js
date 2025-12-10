#!/usr/bin/env node
// Unicode normalization benchmark

function main() {
    const iterations = parseInt(process.argv[2]) || 100000;

    // Test string with mixed NFD and NFC characters
    // NFD form: base characters followed by combining characters
    const testNfd = "cafe\u0301 na\u00EFve re\u0301sume\u0301 coo\u0308perate sen\u0303or";
    // NFC form: precomposed characters
    const testNfc = "caf\u00E9 na\u00EFve r\u00E9sum\u00E9 co\u00F6perate se\u00F1or";

    let totalLen = 0;

    for (let i = 0; i < iterations; i++) {
        // NFC: compose combining sequences
        const normalized1 = testNfd.normalize('NFC');

        // NFD: decompose precomposed characters
        const normalized2 = testNfc.normalize('NFD');

        totalLen += normalized1.length + normalized2.length;
    }

    console.log(totalLen);
}

main();
