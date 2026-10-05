function showSection(sectionId) {

    document.getElementById(sectionId).scrollIntoView({
        behavior: "smooth"
    });
}


// ================= CREATE ACCOUNT =================

async function createAccount() {

    const name = document.getElementById("name").value;
    const age = document.getElementById("age").value;
    const phone = document.getElementById("phone").value;
    const pin = document.getElementById("pin").value;

    if (!name || !age || !phone || !pin) {

        document.getElementById("createResult").innerText =
            "Please fill all fields.";

        return;
    }

    const response = await fetch(
        "http://localhost:8080/create",
        {
            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                name: name,
                age: age,
                phone: phone,
                pin: pin
            })
        }
    );

    const result = await response.text();

    document.getElementById("createResult").innerText =
        result;
}


// ================= DEPOSIT =================

async function depositMoney() {

    const account =
        document.getElementById("depositAccount").value;

    const pin =
        document.getElementById("depositPin").value;

    const amount =
        document.getElementById("depositAmount").value;

    const response = await fetch(
        "http://localhost:8080/deposit",
        {
            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                account: account,
                pin: pin,
                amount: amount
            })
        }
    );

    const result = await response.text();

    document.getElementById("depositResult").innerText =
        result;
}


// ================= WITHDRAW =================

async function withdrawMoney() {

    const account =
        document.getElementById("withdrawAccount").value;

    const pin =
        document.getElementById("withdrawPin").value;

    const amount =
        document.getElementById("withdrawAmount").value;

    const response = await fetch(
        "http://localhost:8080/withdraw",
        {
            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                account: account,
                pin: pin,
                amount: amount
            })
        }
    );

    const result = await response.text();

    document.getElementById("withdrawResult").innerText =
        result;
}


// ================= BALANCE =================

async function checkBalance() {

    const account =
        document.getElementById("balanceAccount").value;

    const pin =
        document.getElementById("balancePin").value;

    const response = await fetch(
        "http://localhost:8080/balance",
        {
            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                account: account,
                pin: pin
            })
        }
    );

    const result = await response.text();

    document.getElementById("balanceResult").innerText =
        result;
}
