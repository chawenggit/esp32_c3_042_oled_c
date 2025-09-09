// to modify for new setting  
const ioFncts = [
    "     ",
    "CntOK",
    "CntNG",

    "Adn_Abnormal",
    "Adn_OFFline",
    "Adn_Confirm",

    "MC_ALMCODE0",
    "MC_ALMCODE1",
    "MC_ALMCODE2",
    "MC_ALMCODE3",
    "MC_ALMCODE4",
    "MC_ALMCODE5",
    "MC_ALMCODE6",
    "MC_ALMCODE7"
];
const mc_id_min = 1;
const mc_id_max = 30;

let isChange = false;
let setting;

let genModuleIO = (data, module_id) => {
    let id_table = (module_id == 0) ? 'id_tbody1' : 'id_tbody2';
    let id_io_ip = (module_id == 0) ? 'io_ip1' : 'io_ip2';
    let id_io_port = (module_id == 0) ? 'io_port1' : 'io_port2';
    let id_enb = (module_id == 0) ? 'enb1' : 'enb2';

    // set enbable 
    document.getElementById(id_enb).selectedIndex = data.module[module_id].enable;

    // empty old tbody
    document.getElementById(id_table).innerHTML = '';
    let ioIp = data.module[module_id].io_ip;
    let ip = ioIp[0] + "." + ioIp[1] + "." + ioIp[2] + "." + ioIp[3]
    document.getElementById(id_io_ip).value = ip
    document.getElementById(id_io_port).value = data.module[module_id].io_port;
    for (let i = 0; i < data.module[module_id].iopin.length; i++) {
        let mc = data.module[module_id].iopin[i].mc;
        let fnc = data.module[module_id].iopin[i].fnc;

        // mc -----------
        let mc_dropdown = "<select class='cls_select mc'><option></option>"
        for (let m = mc_id_min; m <= mc_id_max; m++) {
            mc_dropdown += `<option value=${m}`;
            if (m == mc) {
                mc_dropdown += " selected";
            }
            mc_dropdown += `>${m}</option>`;
        }
        mc_dropdown += "</select>"
        // fnc -----------
        let fnc_dropdown = "<select class='cls_select fnc '>"
        for (let fn = 0; fn < ioFncts.length; fn++) {
            fnc_dropdown += `<option value=${fn}`;
            if (fn == fnc) {
                fnc_dropdown += " selected";
            }
            fnc_dropdown += `>${ioFncts[fn]}</option>`;
        }
        fnc_dropdown += "</select>"

        let table = document.getElementById(id_table);

        // Create a row using the inserRow() method and
        // specify the index where you want to add the row
        let row = table.insertRow(-1); // We are adding at the end

        // Create table cells
        row.insertCell(0).innerHTML = i;
        row.insertCell(1).innerHTML = mc_dropdown;
        row.insertCell(2).innerHTML = fnc_dropdown;

    }
    // const selector = `#${id_table} tr td select`
    // let mod = (id_table == "id_tbody1") ? 0 : 1;
    document.querySelectorAll(`#${id_table} tr td select`).forEach(select => {
        select.onchange = function () {
            let module = (select.parentElement.parentElement.parentElement.id == "id_tbody1") ? 0 : 1;
            let row = parseInt(select.parentElement.parentElement.querySelectorAll('td')[0].innerHTML);
            let newValue = parseInt(select.value);
            if (select.classList.contains('fnc')) {
                setting.module[module].iopin[row].fnc = newValue;
                setModified();
            } else if (select.classList.contains('mc')) {
                setting.module[module].iopin[row].mc = newValue;
                setModified();
            }
            select.style.backgroundColor = "greenyellow";
        }
    });
}

let reloadConfigue = async () => {
    await fetch("/api_get")
        .then((response) => {
            if (response.ok) {
                return response.json();
            }
            else {
                return false;
            }
        })
        .then(async (data) => { // input json
            renderPage(data);
        })
        .catch((err) => {
            console.log("Chaweng reloadConfigue Error", err);
        });
};

let genHmiMcId = (data) => {
    select = document.getElementById('id_hmi_mcid');

    for (let mc = mc_id_min; mc <= mc_id_max; mc++) {
        let opt = document.createElement('option');
        opt.value = mc;
        opt.innerHTML = mc;
        if (mc == data.hmi_mc_id) {
            opt.selected = true;
        }
        select.appendChild(opt);
    }
}

let renderPage = (data) => {
    // console.log(`renderPage data = ${JSON.stringify(data)}`); // todo del
    document.getElementById("firmware_verion").innerHTML = data.firmware_verion;

    let mac_str = ''
    for (let i = 0; i < 6; i++) {
        mac_str += data.pc_mac[i].toString(16).toUpperCase().padStart(2, "0");
        if (i < 5) {
            mac_str += ":"
        }
    }
    document.getElementById("pc_mac").value = mac_str;

    let ip = data.pc_ip[0] + "." + data.pc_ip[1] + "." + data.pc_ip[2] + "." + data.pc_ip[3]
    document.getElementById("pc_ip").value = ip;
    document.getElementById("pc_port").value = data.pc_port;
    genHmiMcId(data);
    
    document.getElementById("id_hc12_ch").value = data.hC12_ch;
    document.getElementById("id_hc12_echo").selectedIndex = data.hC12_echo;
    document.getElementById("id_sleep").value = data.sleep;

    ip = data.os_ip[0] + "." + data.os_ip[1] + "." + data.os_ip[2] + "." + data.os_ip[3]
    document.getElementById("os_ip").value = ip;
    document.getElementById("lineName_id").value = data.lineName;

    genModuleIO(data, 0);
    genModuleIO(data, 1);
    // to modify for new setting = white input
    document.querySelectorAll('input, #id_hc12_echo, #id_hmi_mcid, #id_save, .cls_select ').forEach(elm => {
        elm.style.backgroundColor = "white";
    })

    setting = data;
    isChange = false;
}

document.addEventListener("DOMContentLoaded", () => {
    reloadConfigue();
});

//---------- Preventing page refresh
// to modify for new setting  = onchange
document.getElementById("id_formOsip").addEventListener('submit', (event) => {
    event.preventDefault();
});
document.getElementById("id_formPc").addEventListener('submit', (event) => {
    event.preventDefault();
});
document.getElementById("id_formHC12").addEventListener('submit', (event) => {
    event.preventDefault();
});


// ------------- update mac  -----------
document.getElementById("pc_mac").onchange = function () {

    this.style.backgroundColor = "greenyellow";
    let mac_arr = this.value.split(':')
    for (let i = 0; i < mac_arr.length; i++) {
        setting.pc_mac[i] = parseInt((mac_arr[i]), 16);
    }
    setModified();
}

// ------------- update ip -----------
let updateIp = (element, x_ip) => {
    element.style.backgroundColor = "greenyellow";
    let ip = element.value.split('.')
    for (let i = 0; i < ip.length; i++) {
        x_ip[i] = parseInt(ip[i]);
    }
}

document.getElementById("lineName_id").onchange = function () {
    this.style.backgroundColor = "greenyellow";
    setting.lineName = this.value
    setModified();
}

document.getElementById("os_ip").onchange = function () {
    updateIp(this, setting.os_ip);
    setModified();
}

document.getElementById("pc_ip").onchange = function () {
    updateIp(this, setting.pc_ip);
    setModified();
}

document.getElementById("io_ip1").onchange = function () {
    updateIp(this, setting.module[0].io_ip);
    setModified();
}

document.getElementById("io_ip2").onchange = function () {
    updateIp(this, setting.module[1].io_ip);
    setModified();
}
// ------------- update port -----------

document.getElementById("pc_port").onchange = function () {
    setting.pc_port = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}

document.getElementById("io_port1").onchange = function () {
    setting.module[0].io_port = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}

document.getElementById("io_port2").onchange = function () {
    setting.module[1].io_port = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}

let setModified = () => {
    isChange = true;
    document.getElementById("id_save").style.backgroundColor = "red";
}

document.getElementById("id_save").onclick = async function () {
    if (isChange) {
        await fetch("/api_post", {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(setting)
        })
        reloadConfigue();
    }
}

document.getElementById("id_reboot").onclick = async function () {
    await fetch("/api_reboot", {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ "cmd": "reset" })
        
        
    })
    this.style.backgroundColor = "greenyellow";
}

document.getElementById("id_upload_conf").onclick = function () {
    let fileToLoad = document.getElementById("fileToLoad").files[0];
    let fileReader = new FileReader();
    fileReader.readAsText(fileToLoad, "UTF-8");
    
    fileReader.onload = function (fileLoadedEvent) {
        let jsonStr = fileLoadedEvent.target.result;
        const jsonObj = JSON.parse(jsonStr); 
        renderPage(jsonObj);
        setModified();
    };
}

// from https://thiscouldbebetter.wordpress.com/2012/12/18/loading-editing-and-saving-a-text-file-in-html5-using-javascrip/ 
document.getElementById("id_download_conf").onclick = async function () {
    // get actual data from server
    await reloadConfigue();  // wait until finish loading from server 

    // Transform JSON to string
    const jsonStr = JSON.stringify(setting);
    // Create a Blob object
    const blob = new Blob([jsonStr], { type: 'application/json' });
    const url = URL.createObjectURL(blob);

    let downloadLink = document.createElement("a");  // create download element
    downloadLink.download = 'config.json';  // default file Name
    downloadLink.innerHTML = "Download File";
    downloadLink.href = url;
    downloadLink.onclick = function (event) {
        document.body.removeChild(event.target);
    };
    downloadLink.style.display = "none";
    document.body.appendChild(downloadLink);

    downloadLink.click();
}


// ------------- update enable selector -----------
let updateEnable = (elm, md) => {
    setting.module[md].enable = elm.selectedIndex;
    // console.log(`change enb module = ${md} enable = ${elm.selectedIndex} `);
    elm.style.backgroundColor = "greenyellow";
    setModified();
}

document.getElementById("enb1").onchange = function () {
    updateEnable(this, 0);
}

document.getElementById("enb2").onchange = function () {
    updateEnable(this, 1);
}

document.getElementById("id_hmi_mcid").onchange = function () {
    setting.hmi_mc_id = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}

document.getElementById("id_hc12_ch").onchange = function () {
    setting.hC12_ch = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}

document.getElementById("id_sleep").onchange = function () {
    setting.sleep = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}

document.getElementById("id_hc12_echo").onchange = function () {
    setting.hC12_echo = parseInt(this.value);
    this.style.backgroundColor = "greenyellow";
    setModified();
}
// to modify for new setting  = onchange