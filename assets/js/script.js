/**
 * EPISTULAAO — ページの動き
 * ヒーロー背景の紙片・切手、スクロール表示、ナビの変化
 */
(function () {
  const prefersReducedMotion = window.matchMedia(
    '(prefers-reduced-motion: reduce)'
  ).matches;

  /* ---------- ヒーロー：浮かぶ紙と切手（Canvas） ---------- */
  function initHeroCanvas() {
    const hero = document.querySelector('.hero');
    if (!hero) return;

    const canvas = document.createElement('canvas');
    canvas.className = 'hero-canvas';
    canvas.setAttribute('aria-hidden', 'true');
    hero.prepend(canvas);

    const ctx = canvas.getContext('2d');
    const particles = [];
    const count = 18;

    function resize() {
      const rect = hero.getBoundingClientRect();
      const dpr = Math.min(window.devicePixelRatio || 1, 2);
      canvas.width = rect.width * dpr;
      canvas.height = rect.height * dpr;
      canvas.style.width = `${rect.width}px`;
      canvas.style.height = `${rect.height}px`;
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    }

    function randomBetween(min, max) {
      return min + Math.random() * (max - min);
    }

    function createParticle() {
      const w = canvas.width / (window.devicePixelRatio || 1);
      const h = canvas.height / (window.devicePixelRatio || 1);
      const isStamp = Math.random() < 0.35;

      return {
        x: randomBetween(0, w),
        y: randomBetween(0, h),
        vx: randomBetween(-0.15, 0.15),
        vy: randomBetween(-0.25, -0.05),
        rotation: randomBetween(0, Math.PI * 2),
        spin: randomBetween(-0.004, 0.004),
        size: isStamp ? randomBetween(10, 18) : randomBetween(14, 32),
        aspect: isStamp ? 1 : randomBetween(1.2, 1.7),
        isStamp,
        opacity: randomBetween(0.08, 0.22),
      };
    }

    function drawPaper(p) {
      ctx.save();
      ctx.translate(p.x, p.y);
      ctx.rotate(p.rotation);
      ctx.globalAlpha = p.opacity;
      ctx.fillStyle = '#ffffff';
      ctx.strokeStyle = 'rgba(42, 42, 40, 0.12)';
      ctx.lineWidth = 0.8;
      const pw = p.size * p.aspect;
      const ph = p.size;
      ctx.fillRect(-pw / 2, -ph / 2, pw, ph);
      ctx.strokeRect(-pw / 2, -ph / 2, pw, ph);
      ctx.restore();
    }

    function drawStamp(p) {
      ctx.save();
      ctx.translate(p.x, p.y);
      ctx.rotate(p.rotation);
      ctx.globalAlpha = p.opacity + 0.05;
      ctx.fillStyle = '#eba0a0';
      ctx.strokeStyle = 'rgba(42, 42, 40, 0.2)';
      ctx.lineWidth = 1;
      const r = p.size / 2;
      ctx.beginPath();
      ctx.arc(0, 0, r, 0, Math.PI * 2);
      ctx.fill();
      ctx.stroke();
      ctx.restore();
    }

    function tick() {
      const w = canvas.width / (window.devicePixelRatio || 1);
      const h = canvas.height / (window.devicePixelRatio || 1);

      ctx.clearRect(0, 0, w, h);

      for (const p of particles) {
        p.x += p.vx;
        p.y += p.vy;
        p.rotation += p.spin;

        if (p.y < -40) {
          p.y = h + 40;
          p.x = randomBetween(0, w);
        }
        if (p.x < -40) p.x = w + 40;
        if (p.x > w + 40) p.x = -40;

        if (p.isStamp) drawStamp(p);
        else drawPaper(p);
      }

      requestAnimationFrame(tick);
    }

    resize();
    for (let i = 0; i < count; i++) particles.push(createParticle());

    window.addEventListener('resize', resize);
    requestAnimationFrame(tick);
  }

  /* ---------- スクロールでふわっと表示 ---------- */
  function initScrollReveal() {
    const heroTargets = document.querySelectorAll(
      '.hero .js-reveal'
    );
    const scrollTargets = document.querySelectorAll(
      '.section.js-reveal, .work-card.js-reveal, .article .js-reveal'
    );

    const show = (el) => el.classList.add('is-visible');

    if (prefersReducedMotion) {
      heroTargets.forEach(show);
      scrollTargets.forEach(show);
      return;
    }

    /* ヒーローは読み込み直後から順に表示 */
    heroTargets.forEach((el, i) => {
      setTimeout(() => show(el), 100 * i);
    });

    const observer = new IntersectionObserver(
      (entries) => {
        entries.forEach((entry) => {
          if (entry.isIntersecting) {
            show(entry.target);
            observer.unobserve(entry.target);
          }
        });
      },
      { threshold: 0.1, rootMargin: '0px 0px -8% 0px' }
    );

    scrollTargets.forEach((el) => observer.observe(el));
  }

  /* ---------- スクロールでナビに影 ---------- */
  function initNavScroll() {
    const nav = document.querySelector('.nav');
    if (!nav) return;

    const onScroll = () => {
      nav.classList.toggle('nav--scrolled', window.scrollY > 16);
    };

    onScroll();
    window.addEventListener('scroll', onScroll, { passive: true });
  }

  /* ---------- ヒーローロゴ：ゆらゆら ---------- */
  function initHeroLogoFloat() {
    const logo = document.querySelector('.hero-logo');
    if (!logo || prefersReducedMotion) return;
    logo.classList.add('hero-logo--float');
  }

  function boot() {
    document.documentElement.classList.add('js-enabled');
    initScrollReveal();
    initNavScroll();
    initHeroLogoFloat();
    if (!prefersReducedMotion) initHeroCanvas();
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', boot);
  } else {
    boot();
  }
})();
