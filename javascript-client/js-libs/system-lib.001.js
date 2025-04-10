function getRunSimulationURL(){         return "https://www.dcc.ufrrj.br/marcelo-z/api-run-simulation.php"}
function getRunAuthURL(){               return "https://www.dcc.ufrrj.br/marcelo-z/auth.php"}
/*
 * <!Brief return the const name of private key stored in client. This function is shared by some php/html files
 */
function getKeyName(){                  return "privateKeyPem-SEIR-Model"; }

async function authenticate() {
   const key_name = getKeyName();
   const privateKeyPem = localStorage.getItem(key_name);


    if (!privateKeyPem) {
        alert("Please load your private key first.");
        return;
    }

    const privateKey = forge.pki.privateKeyFromPem(privateKeyPem);
    const message = `id::${Date.now()}`; // Unique message (can also be a nonce from the server)
    const md = forge.md.sha256.create();
    md.update(message, "utf8");

    const signature = privateKey.sign(md);

    const signatureBase64 = forge.util.encode64(signature);

    // Send to server
    const response = await fetch(getRunSimulationURL(), {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            message: message,
            signature: signatureBase64
        })
    });

    const result = await response.text();
    console.log("Server says:", result);
}



/*
 * <!Brief
 */
function main_on_load_script(){
    // Set default values/initial condition
    const date = Date.now()
    const st_simulation = date + "-SEIR-Model"
    document.getElementById("id_simulationName").value = st_simulation;
    document.getElementById("id_numNodes").value = 10000;
    document.getElementById("id_cellsX").value = 100;
    document.getElementById("id_cellsY").value = 100;
    document.getElementById("id_power").value = 1;
    document.getElementById("id_minConnected").value = 6;
    document.getElementById("id_infected").value = 0.01;
    document.getElementById("id_timesteps").value = 35;

    document.getElementById("id_alphaMi").value = 1.5;
    document.getElementById("id_alphaSig").value = 0.25;
    document.getElementById("id_gammaMi").value = 2;
    document.getElementById("id_gammaSig").value = 0.75;

}

/*
 * <!Brief
 */

function remove_key(){
        const key_name = getKeyName() ;
        console.log("remove_key", localStorage.getItem(key_name));
        // Optional: check if it was removed
        if (localStorage.getItem(key_name)){
            localStorage.removeItem(key_name);
            alert("Key has been removed!");
        }else{
            alert("there is no key!");
        }
}

/*
 * <!Brief
 */

function install_key(){
    const key_name = getKeyName() ;
    if (localStorage.getItem(key_name)){
        alert("Key has been installed!");
        return;
    }

    const popup = window.open("load-key.html", "Popup", "width=640,height=480");
    window.addEventListener("message", (event) => {
        if (event.origin !== window.location.origin) return; // safety check
        console.log("Received from popup:", event.data);
    });
}
async function main_run_simulation(){
    //Get form info
    const key_name = getKeyName();
    const privateKeyPem = localStorage.getItem(key_name);


    if (!privateKeyPem) {
        alert("Please load your private key first.");
        return;
    }

    const privateKey = forge.pki.privateKeyFromPem(privateKeyPem);
    const message = `id::${Date.now()}`; // Unique message (can also be a nonce from the server)
    const md = forge.md.sha256.create();
    md.update(message, "utf8");

    const signature = privateKey.sign(md);

    const signatureBase64 = forge.util.encode64(signature);

    //------------------------------------------------------
        const username = document.getElementById("id_Login").value;

        const dataEnconded = {
            simulationName: document.getElementById("id_simulationName").value,
            numNodes: parseInt(document.getElementById("id_numNodes").value),
            power: parseFloat(document.getElementById("id_power").value),
            minConnected: parseInt(document.getElementById("id_minConnected").value),
            cellsX: parseInt(document.getElementById("id_cellsX").value),
            cellsY: parseInt(document.getElementById("id_cellsY").value),
            infected: parseInt(document.getElementById("id_infected").value),
            timesteps: parseInt(document.getElementById("id_timesteps").value),
            infectedOrder: document.getElementById("infectedOrder").value,
            alphaMi: parseFloat(document.getElementById("id_alphaMi").value),
            alphaSig: parseFloat(document.getElementById("id_alphaSig").value),
            gammaMi: parseFloat(document.getElementById("id_gammaMi").value),
            gammaSig: parseFloat(document.getElementById("id_gammaSig").value),
            verbose: document.getElementById("verbose").checked
        };
         const dataJson = JSON.stringify(dataEnconded, null, 2);

    // Send to server
    const response = await fetch(getRunSimulationURL(), {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            message: message,
            user: username,
            param:dataJson,
            signature: signatureBase64
        })
    });

    const result = await response.text();
    console.log("Server says:", result);
}


/*
 * <!Brief
 */

async function main_start_simulation(){
        const user = document.getElementById("id_Login").value;

        console.log("user", user);
        const dataEnconded = {
            simulationName: document.getElementById("id_simulationName").value,
            numNodes: parseInt(document.getElementById("id_numNodes").value),
            power: parseFloat(document.getElementById("id_power").value),
            minConnected: parseInt(document.getElementById("id_minConnected").value),
            cellsX: parseInt(document.getElementById("id_cellsX").value),
            cellsY: parseInt(document.getElementById("id_cellsY").value),
            infected: parseInt(document.getElementById("id_infected").value),
            timesteps: parseInt(document.getElementById("id_timesteps").value),
            infectedOrder: document.getElementById("infectedOrder").value,
            alphaMi: parseFloat(document.getElementById("id_alphaMi").value),
            alphaSig: parseFloat(document.getElementById("id_alphaSig").value),
            gammaMi: parseFloat(document.getElementById("id_gammaMi").value),
            gammaSig: parseFloat(document.getElementById("id_gammaSig").value),
            verbose: document.getElementById("verbose").checked
        };
        authenticate(user)
        const dataJson = JSON.stringify(dataEnconded, null, 2);

        console.log("Json file:", dataJson);


}
