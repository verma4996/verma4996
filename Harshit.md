<!DOCTYPE html>
<html>
<head>
    <title>IEEE 754 Converter</title>
    <style>
        body {
            font-family: sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f4f4f4;
        }

        .container {
            width: 400px;
            padding: 20px;
            background-color: #fff;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .input-section {
            margin-bottom: 15px;
        }

        label {
            display: block;
            margin-bottom: 5px;
        }

        input[type="number"], input[type="text"], textarea {
            width: 100%;
            padding: 8px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        button {
            padding: 8px 15px;
            background-color: #007bff;
            color: #fff;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }

        button:hover {
            background-color: #0056b3;
        }

        .output-section {
            margin-top: 20px;
        }

        textarea {
            height: 100px;
            resize: vertical;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>IEEE 754 32-bit Converter</h1>

        <div class="input-section">
            <label for="decimalInput">Decimal:</label>
            <input type="number" id="decimalInput" placeholder="Enter decimal">
            <button id="decimalToBinary">Convert to Binary</button>
        </div>

        <div class="input-section">
            <label for="binaryInput">Binary (32 bits):</label>
            <input type="text" id="binaryInput" placeholder="Enter 32-bit binary">
            <button id="binaryToDecimal">Convert to Decimal</button>
        </div>

        <div class="output-section">
            <label>Output:</label>
            <textarea id="output" readonly></textarea>
        </div>
    </div>

    <script>
        document.getElementById('decimalToBinary').addEventListener('click', () => {
            const decimal = parseFloat(document.getElementById('decimalInput').value);
            if (isNaN(decimal)) {
                document.getElementById('output').value = 'Invalid decimal input.';
                return;
            }
            const binary = decimalToIEEE754(decimal);
            document.getElementById('output').value = binary;
        });

        document.getElementById('binaryToDecimal').addEventListener('click', () => {
            const binary = document.getElementById('binaryInput').value;
            if (binary.length !== 32 || !/^[01]+$/.test(binary)) {
                document.getElementById('output').value = 'Invalid 32-bit binary input.';
                return;
            }
            const decimal = ieee754ToDecimal(binary);
            document.getElementById('output').value = decimal;
        });

        function decimalToIEEE754(decimal) {
            const sign = decimal < 0 ? '1' : '0';
            const absDecimal = Math.abs(decimal);

            if (absDecimal === 0) {
                return '00000000000000000000000000000000';
            }

            if (absDecimal === Infinity) {
                return sign + '1111111100000000000000000000000';
            }

            if (isNaN(absDecimal)){
                return '01111111110000000000000000000001'; //NaN
            }

            let exponent = 0;
            let mantissa = '';

            if (absDecimal >= 1) {
                exponent = Math.floor(Math.log2(absDecimal));
                mantissa = (absDecimal / Math.pow(2, exponent)).toString(2).substring(2);
            } else {
                exponent = Math.floor(Math.log2(absDecimal));
                mantissa = (absDecimal / Math.pow(2, exponent)).toString(2).substring(2);
            }

            const biasedExponent = (exponent + 127).toString(2).padStart(8, '0');
            mantissa = mantissa.padEnd(23, '0').substring(0, 23);

            return sign + biasedExponent + mantissa;
        }

        function ieee754ToDecimal(binary) {
            const sign = binary[0] === '1' ? -1 : 1;
            const exponent = parseInt(binary.substring(1, 9), 2) - 127;
            const mantissa = '1' + binary.substring(9);

            if (exponent === 128) {
                if (mantissa.substring(1) === '00000000000000000000000'){
                    return sign === -1 ? -Infinity: Infinity;
                } else {
                    return NaN;
                }
            }

            if (exponent === -127) {
                let decimal = 0;
                for (let i = 1; i < mantissa.length; i++) {
                    decimal += parseInt(mantissa[i]) * Math.pow(2, -i);
                }
                return sign * decimal;
            }

            let decimal = 1;
            for (let i = 1; i < mantissa.length; i++) {
                decimal += parseInt(mantissa[i]) * Math.pow(2, -i);
            }
            return sign * decimal * Math.pow(2, exponent);
        }
    </script>
</body>
</html>
