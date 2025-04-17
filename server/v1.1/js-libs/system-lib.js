/**
 * @file system-lib.js
 * @brief Utility functions for SEIR Model simulation.
 */

/**
 * @function getRunSimulationURL
 * @brief Inform the link of simulation.
 * @returns {string} URL of simulation.
 */
function getRunSimulationURL(){         return "https://www.dcc.ufrrj.br/marcelo-z/~seir/api-run-simulation.php"}

/**
 * @function getKeyName
 * @brief Inform file private key on client browser.
 * @returns {string} private key file name.
 */
function getKeyName(){                  return "privateKeyPem-SEIR-Model"; }


/**
 * @function main_on_load_script
 * @brief This function is responsible for filling the initial parameters of the simulation.Inform file private key on client browser.
 */
function main_on_load_script(){
    // Set default values/initial condition
    //const date = Date.now()
    //const st_simulation = date + "-SEIR-Model"
    //document.getElementById("id_simulationName").value = st_simulation;
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


/**
 * @function remove_key
 * @brief Removes key from client browser
 */
function remove_key(){
        const key_name = getKeyName() ;
        //console.log("remove_key", localStorage.getItem(key_name));
        // Optional: check if it was removed
        if (localStorage.getItem(key_name)){
            localStorage.removeItem(key_name);
            alert("Key has been removed!");
        }else{
            alert("there is no key!");
        }
}

/**
 * @function install_key
 * @brief Installs key from client browser
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
        //console.log("Received from popup:", event.data);
    });
}

/**
 * @function download Json config file
 * @brief
 */
function downloadJSON() {
    const instance = "BA-N-" + document.getElementById("id_numNodes").value +
                    "-P-"   + document.getElementById("id_power").value +
                    "-C-"   + document.getElementById("id_minConnected").value + ".net";

    //const currentdate = new Date();
    //const simu_id =   currentdate.getFullYear() + "-" + (currentdate.getMonth()+1) + "-"
    //                + currentdate.getDate() + "-" + currentdate.getHours()
    //                + currentdate.getMinutes() + currentdate.getSeconds();
    const current = new Date().toISOString();
    const simu_id = current.replace(new RegExp(":", "g"), "-");
    const defaultFilename = "SEIR-Model-" + simu_id + ".json";
    const global_param = JSON.stringify({
                         "global-params":{
                                "simulation-name": "SEIR-Model-" + simu_id,
                                "instance": instance,
                                "verbose": document.getElementById("verbose").checked,
                                "cell-x": parseInt(document.getElementById("id_cellsX").value),
                                "cell-y":  parseInt(document.getElementById("id_cellsY").value),
                                "infected": parseFloat(document.getElementById("id_infected").value),
                                "timesteps": parseInt(document.getElementById("id_timesteps").value),
                                "infected_init_order": document.getElementById("infectedOrder").value,
                                "alpha-rumor-mi": parseFloat(document.getElementById("id_alphaMi").value),
                                "alpha-rumor-sig": parseFloat(document.getElementById("id_alphaSig").value),
                                "gamma-rumor-mi": parseFloat(document.getElementById("id_gammaMi").value),
                                "gamma-rumor-sig": parseFloat(document.getElementById("id_gammaSig").value)
                         }

                    }, null, 3);



    const filename = prompt("Enter the filename:", defaultFilename);
    if (!filename) return; // user canceled

    const blob = new Blob([global_param], { type: 'application/json;charset=utf-8;' });
    const url = URL.createObjectURL(blob);

    const link = document.createElement("a");
    link.setAttribute("href", url);
    link.setAttribute("download", filename);
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
}

/**
 * @function download CSV
 * @brief
 */
function downloadCSV() {
  const csvData =  document.getElementById("id_simulation_output").value;

  const defaultFilename = "simulation_data.csv";
  const filename = prompt("Enter the filename:", defaultFilename);
  if (!filename) return; // user canceled

  const blob = new Blob([csvData], { type: 'text/csv;charset=utf-8;' });
  const url = URL.createObjectURL(blob);

  const link = document.createElement("a");
  link.setAttribute("href", url);
  link.setAttribute("download", filename);
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
}
/**
 *
 * @brief Global variable so that javascript can keep reference to variable and also can destroy it when necessary.
 */
let chartInstance = null;
/**
 * @function main_run_simulation
 * @brief This function performs authentication with the server.
 */
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
            numNodes: parseInt(document.getElementById("id_numNodes").value),
            power: parseFloat(document.getElementById("id_power").value),
            minConnected: parseInt(document.getElementById("id_minConnected").value),
            cellsX: parseInt(document.getElementById("id_cellsX").value),
            cellsY: parseInt(document.getElementById("id_cellsY").value),
            infected: parseFloat(document.getElementById("id_infected").value),
            timesteps: parseInt(document.getElementById("id_timesteps").value),
            infectedOrder: document.getElementById("infectedOrder").value,
            alphaMi: parseFloat(document.getElementById("id_alphaMi").value),
            alphaSig: parseFloat(document.getElementById("id_alphaSig").value),
            gammaMi: parseFloat(document.getElementById("id_gammaMi").value),
            gammaSig: parseFloat(document.getElementById("id_gammaSig").value),
            verbose: document.getElementById("verbose").checked
        };
        //console.log("Part 1", dataEnconded);
        //const dataJson = JSON.stringify(dataEnconded, null, 2);

        //UI component to inform the users that the server is working
        const spinner = document.getElementById('loadingSpinner');
         // Show spinner
        spinner.classList.remove('d-none');
        try {//Running the simulation
        //--------------------------------------------
        // Send to server
                const response = await fetch(getRunSimulationURL(), {
                    method: "POST",
                    headers: {
                        "Content-Type": "application/json"
                    },
                    body: JSON.stringify({
                        message: message,
                        user: username,
                        params:dataEnconded,
                        signature: signatureBase64
                    })
                });

                //Get server answer
                const result = await response.text();
                const data =  JSON.parse(result);
                const status = data.status;
                console.log("Server status: ", status);
                if (status == "OK simulation"){
                    document.getElementById('id_output_div').classList.replace("d-none", "d-block");
                    const csvData = data.data_simulation;
                    document.getElementById("id_simulation_output").value = csvData;
                    // Parse CSV
                    const lines = csvData.trim().split("\n");
                    const headers = lines[0].split(";");

                    const time = [];
                    const susceptible = [];
                    const exposed = [];
                    const infected = [];
                    const recovered = [];

                    lines.slice(1).forEach(line => {
                    const parts = line.split(";");
                    time.push(parts[0]);
                    susceptible.push(parts[1]);
                    exposed.push(parts[2]);
                    infected.push(parts[3]);
                    recovered.push(parts[4]);
                    });

                    const ctx = document.getElementById('simulationChart').getContext('2d');
                    if (chartInstance)  chartInstance.destroy();
                    chartInstance = new Chart(ctx, {
                            type: 'line',
                            data: {
                                labels: time,
                                datasets: [
                                {
                                    label: 'Susceptible',
                                    data: susceptible,
                                    borderColor: 'blue',
                                    fill: false
                                },
                                {
                                    label: 'Exposed',
                                    data: exposed,
                                    borderColor: 'orange',
                                    fill: false
                                },
                                {
                                    label: 'Infected',
                                    data: infected,
                                    borderColor: 'red',
                                    fill: false
                                },
                                {
                                    label: 'Recovered',
                                    data: recovered,
                                    borderColor: 'green',
                                    fill: false
                                }
                                ]
                            },
                            options: {
                                responsive: true,
                                scales: {
                                x: {
                                    title: {
                                    display: true,
                                    text: 'Time'
                                    }
                                },
                                y: {
                                    title: {
                                    display: true,
                                    text: 'Individuals'
                                    },
                                    beginAtZero: true
                                }
                                }
                            }
                            });
                        //console.log("Simulation:", data.data_simulation);
                        //document.getElementById("id_simulation_output").value = data.data_simulation;

                }//if (status == "OK simulation"){


        //--------------------------------------------

        } catch (error) {
            console.error('Error during simulation:', error);
            alert('Simulation failed.');
        } finally {
            // Hide spinner
            spinner.classList.add('d-none');
        }




}//async function main_run_simulation(){


