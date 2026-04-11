/** @type {import('next').NextConfig} */
const nextConfig = {}

const path = require('path');

module.exports = {
  ...nextConfig,
  webpack: (config) => {
    config.resolve.alias = {
      ...config.resolve.alias,
      '@': path.resolve(__dirname), // Define o alias @ para a raiz do projeto
    };
    return config;
  },
};
