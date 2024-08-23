// Import and configure Firebase
import { initializeApp } from "https://www.gstatic.com/firebasejs/9.22.2/firebase-app.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/9.22.2/firebase-database.js";

// Your Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyD7ad1sX0X1cVS8Q1P5L9XUKcNe-9OByVM",
    authDomain: "cat-monitoring-project.firebaseapp.com",
    databaseURL: "https://cat-monitoring-project-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "cat-monitoring-project",
    storageBucket: "cat-monitoring-project.appspot.com",
    messagingSenderId: "367404636678",  // GCM_SENDER_ID
    appId: "1:367404636678:web:897bcfc415375ae1a34187"  // GOOGLE_APP_ID
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// References to the Firebase database
const temperatureRef = ref(database, 'temperature');
const humidityRef = ref(database, 'humidity');
const distanceRef = ref(database, 'distance');
const statusRef = ref(database, 'status');
const heatStrokePercentageRef = ref(database, 'heatStrokePercentage');

// Update the page with data from Firebase
function updateData() {
    onValue(temperatureRef, (snapshot) => {
        const temperature = snapshot.val();
        document.getElementById('temperature').innerText = temperature ? temperature.toFixed(1) : '--';
    });

    onValue(humidityRef, (snapshot) => {
        const humidity = snapshot.val();
        document.getElementById('humidity').innerText = humidity ? humidity.toFixed(1) : '--';
    });

    onValue(distanceRef, (snapshot) => {
        const distance = snapshot.val();
        document.getElementById('distance').innerText = distance ? distance.toFixed(1) : '--';
    });

    onValue(statusRef, (snapshot) => {
        const status = snapshot.val();
        document.getElementById('statusText').innerText = status ? status : 'Loading...';
        updateStatusColor(status);
    });

    onValue(heatStrokePercentageRef, (snapshot) => {
        const heatStrokePercentage = snapshot.val();
        document.getElementById('heatStrokePercentage').innerText = heatStrokePercentage ? heatStrokePercentage.toFixed(1) : '--';
    });
}

// Function to change color based on status
function updateStatusColor(status) {
    const statusText = document.getElementById('statusText');
    if (status === 'Danger') {
        statusText.style.color = '#ff0000';
    } else if (status === 'Hot') {
        statusText.style.color = '#ff6600';
    } else if (status === 'Comfort') {
        statusText.style.color = '#00cc00';
    } else {
        statusText.style.color = '#333333';
    }
}

// Start updating data
updateData();
