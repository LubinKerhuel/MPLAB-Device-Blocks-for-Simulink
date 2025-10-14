// Navigation Panel JavaScript for MCHP Blockset Documentation

(function() {
    'use strict';

    // Initialize navigation when DOM is loaded
    document.addEventListener('DOMContentLoaded', function() {
        initNavigation();
        highlightCurrentPage();
        expandActivePath();
    });

    // Initialize navigation functionality
    function initNavigation() {
        // Category toggle functionality
        const categories = document.querySelectorAll('.nav-category');
        categories.forEach(function(category) {
            category.addEventListener('click', function(e) {
                e.preventDefault();
                toggleCategory(this);
            });
        });

        // Subcategory toggle functionality
        const subcategories = document.querySelectorAll('.nav-subcategory');
        subcategories.forEach(function(subcategory) {
            subcategory.addEventListener('click', function(e) {
                e.preventDefault();
                toggleSubcategory(this);
            });
        });

        // Mobile navigation toggle
        const navToggle = document.querySelector('.nav-toggle');
        if (navToggle) {
            navToggle.addEventListener('click', function() {
                toggleMobileNav();
            });
        }

        // Close mobile nav when clicking outside
        document.addEventListener('click', function(e) {
            const navContainer = document.querySelector('.doc-nav-container');
            const navToggle = document.querySelector('.nav-toggle');

            if (window.innerWidth <= 768 &&
                navContainer &&
                navContainer.classList.contains('open') &&
                !navContainer.contains(e.target) &&
                !navToggle.contains(e.target)) {
                toggleMobileNav();
            }
        });
    }

    // Toggle category expand/collapse
    function toggleCategory(categoryElement) {
        const navList = categoryElement.nextElementSibling;
        if (navList && navList.classList.contains('nav-list')) {
            categoryElement.classList.toggle('collapsed');
            navList.classList.toggle('collapsed');
        }
    }

    // Toggle subcategory expand/collapse
    function toggleSubcategory(subcategoryElement) {
        const navSublist = subcategoryElement.nextElementSibling;
        if (navSublist && navSublist.classList.contains('nav-sublist')) {
            subcategoryElement.classList.toggle('collapsed');
            navSublist.classList.toggle('collapsed');
        }
    }

    // Toggle mobile navigation
    function toggleMobileNav() {
        const navContainer = document.querySelector('.doc-nav-container');
        const body = document.body;

        if (navContainer) {
            navContainer.classList.toggle('open');
            body.classList.toggle('nav-open');
        }
    }

    // Highlight current page in navigation
    function highlightCurrentPage() {
        const currentPath = window.location.pathname;
        const navLinks = document.querySelectorAll('.nav-link');

        navLinks.forEach(function(link) {
            const linkPath = link.getAttribute('href');

            // Normalize paths for comparison
            const normalizedCurrent = normalizePath(currentPath);
            const normalizedLink = normalizePath(linkPath);

            if (normalizedCurrent === normalizedLink) {
                link.classList.add('active');
            }
        });
    }

    // Expand navigation path to show current page
    function expandActivePath() {
        const activeLink = document.querySelector('.nav-link.active');
        if (!activeLink) return;

        // Expand all parent lists
        let parent = activeLink.parentElement;
        while (parent) {
            if (parent.classList.contains('nav-list') || parent.classList.contains('nav-sublist')) {
                parent.classList.remove('collapsed');

                // Expand the category/subcategory header
                const header = parent.previousElementSibling;
                if (header && (header.classList.contains('nav-category') || header.classList.contains('nav-subcategory'))) {
                    header.classList.remove('collapsed');
                }
            }
            parent = parent.parentElement;
        }
    }

    // Normalize path for comparison
    function normalizePath(path) {
        if (!path) return '';

        // Remove leading slash and trailing slash
        path = path.replace(/^\/+|\/+$/g, '');

        // Handle relative paths
        if (!path.startsWith('http')) {
            // Get current directory depth
            const currentPath = window.location.pathname;
            const depth = (currentPath.match(/\//g) || []).length - 1;

            // Remove ../ prefixes and adjust
            const upLevels = (path.match(/\.\.\//g) || []).length;
            path = path.replace(/\.\.\//g, '');

            // Build absolute path
            const pathParts = currentPath.split('/').filter(Boolean);
            pathParts.splice(-1, 1); // Remove filename
            pathParts.splice(-(upLevels), upLevels); // Go up directories

            if (path) {
                pathParts.push(path);
            }

            path = pathParts.join('/');
        }

        return path.toLowerCase();
    }

    // Handle window resize
    window.addEventListener('resize', function() {
        if (window.innerWidth > 768) {
            const navContainer = document.querySelector('.doc-nav-container');
            const body = document.body;

            if (navContainer) {
                navContainer.classList.remove('open');
            }
            if (body) {
                body.classList.remove('nav-open');
            }
        }
    });

})();
