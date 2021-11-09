const fs = require('fs');
const path = require('path');

const startPath = '../../2021_ite2038_2018008340.wiki';

const fileNameMapping = {};
const linkMapping = {};

function recursive(dirPath, cb = null, relativePath = '/') {
  const files = fs.readdirSync(dirPath, { withFileTypes: true });
  files.forEach(file => {
    if(file.isDirectory()) {
      if(path.basename(dirPath) === '.git') return;
      recursive(
        path.join(dirPath, file.name),
        cb,
        path.join(relativePath, file.name)
      );
    }
    else if(cb) {
      if(path.extname(file.name) !== '.md') return;
      cb(dirPath, file, relativePath);
    }
  })
}

recursive(startPath, (dirPath, file, relativePath) => {
  const filePath = path.join(dirPath, file.name);
  const relativeFilePath = path.join(relativePath, file.name);

  const fileContent = fs.readFileSync(filePath, { encoding: 'utf8' });
  const yamlHeader = fileContent.match(/(?<=^---).*?(?=---)/s);

  if(yamlHeader) {
    const yamlHeaderString = yamlHeader[0];
    const title = yamlHeaderString.match(/(?<=title: ).*/);

    fileNameMapping[filePath] = path.join(dirPath, `${title[0]}.md`);
    linkMapping[relativeFilePath.replace(/^\//, '')] = path.join(relativePath, title[0]);
    linkMapping[relativeFilePath.replace(/\.md$/, '')] = path.join(relativePath, title[0]);
  }

  console.log(`Scan file : ${filePath}`);
});

console.log(linkMapping);

recursive(startPath, (dirPath, file) => {
  const filePath = path.join(dirPath, file.name);

  let fileContent = fs.readFileSync(filePath, { encoding: 'utf8' });

  // Remove YAML Header
  fileContent = fileContent.replace(/^---.*?---/sg, '');

  // Rename files for its title
  Object.keys(linkMapping).forEach(link => {
    const linkResult = linkMapping[link];
    fileContent = fileContent.replace(new RegExp(link.replace(/[.]/g, `\\.`), 'g'), linkResult);
  });

  // Fix dashed link into underscored.
  fileContent = fileContent.replace(/\[(.+?)\]\((.+?)\)/g, (_, linkName) => {
    const wrongLink = linkName.toLowerCase().replace(/_/g, '-');
    return _.replace(wrongLink, linkName.toLowerCase());
  });

  // Write data
  fs.writeFileSync(filePath, fileContent);

  console.log(`Rewrited file : ${filePath}`);
});

Object.keys(fileNameMapping).forEach(originalFile => {
  if(!fs.existsSync(path.dirname(fileNameMapping[originalFile])))
    fs.mkdirSync(path.dirname(fileNameMapping[originalFile]), { recursive: true });
  fs.renameSync(originalFile, fileNameMapping[originalFile]);
});

const homePath = path.join(startPath, 'home.md');

const homeFile = fs.readFileSync(homePath, { encoding: 'utf8' });
const homeParts = homeFile.split(/\n(?=[^ ])/sg);

const realignedParts = ['# DB Project wiki\n'];

const remainingParts = homeParts.map(part => {
  return part.replace('*', '###');
}).filter(part => {
  const partTitle = part.match(/### \[(.+?)\]/)[1];

  if(partTitle === 'Modules') {
    realignedParts.push(part);
    realignedParts.push('***');
  }

  return partTitle !== 'Modules';
});

realignedParts.push(...remainingParts);

fs.writeFileSync(homePath, realignedParts.join('\n'));