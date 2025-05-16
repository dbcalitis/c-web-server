const contentDiv = document.getElementById("content");

function displayProjects() {
  let projects;
  fetch('../data/projects.json')  // Go up one level to access the JSON file
    .then(response => response.json()) // Parse JSON response
    .then(data => {
      const mainContainer = document.querySelector('content');
      const projectsGrid = document.createElement('div');
      contentDiv.innerHTML = "";

      for (let i = 0; i < data.length; i++) {
        projectsGrid.innerHTML += `
        <div class="project">
        <h2>${data[i].title}</h2>
        <p>${data[i].description}</p>
        <a href=${data[i].link} target="_blank">Go to Project<a>
        </div>`;
      }

      contentDiv.appendChild(projectsGrid);
    })
    .catch(error => console.error("Error fetching JSON:", error));

}

function displayPosts() {
  const posts = [
    {
      title: "Post 1: Honeypots",
      excerpt: "An intro to honeypots, honeytokens, and canary tokens.",
      slug: "1"
    },
  ];

  const postsContainer = document.createElement('div');
  postsContainer.classList.add('posts-grid');
  contentDiv.innerHTML = "";

  posts.forEach(post => {
    postsContainer.innerHTML += `
      <div class="post">
        <h2>${post.title}</h2>
        <p>${post.excerpt}</p>
        <a href="blog/post.html?${post.slug}" target="_blank">Read More</a>
      </div>
    `;
  });

  contentDiv.appendChild(postsContainer);
}

function updateContent() {
  let hash = window.location.hash.substring(1);
  let contentElement = document.getElementById("content");

  let contentMap = {
    "": `<img src="images/clouds.jpg" alt="Clouds" id="clouds"/>`,
    "about": `<div class="textbox">
      <p>Hi! I am an Honours Computer Science Co-op Student in the Cybersecurity Stream at Carleton University in Ottawa, Ontario, Canada. When I am not applying for jobs and constantly updating my resume, I enjoy travelling, taking walks outdoors, and trying new foods. This website serves as my research blog and documentation of my learning.</p></div>`,
    "blog": `<div class="textbox">
      <p>Coming soon. (First post will be on May 21 :])</p></div>`
  }

  if (hash === "projects") {
    displayProjects();
  }/* else if (hash === "blog") {
    displayPosts();
  }*/ else {
    contentElement.innerHTML = contentMap[hash] || "Page not found.";
  }
}

window.addEventListener("hashchange", updateContent);
window.addEventListener("load", updateContent);

document.addEventListener("DOMContentLoaded", function () {
  const timeDisplay = document.getElementById("time");
  
  if (timeDisplay) {
    const timestamp = Date.now(); 
    const currentDate = new Date(timestamp);
    const formattedDate = currentDate.toLocaleString();

    timeDisplay.innerHTML = "♡ "+ formattedDate;

    setInterval(function () {
      const timestamp = Date.now(); 
      const currentDate = new Date(timestamp);
      const formattedDate = currentDate.toLocaleString();

      timeDisplay.innerHTML = "♡ " + formattedDate;

    }, 1000);
  }

});
