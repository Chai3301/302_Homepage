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

  /* ---------- 慣性スクロール（ホイール / トラックパッド） ---------- */
  function initInertialScroll() {
    if (prefersReducedMotion) return;

    /* スマホなどタッチ主体の端末のみ除外（タッチ付き PC は対象） */
    const isTouchPrimary = window.matchMedia(
      '(hover: none) and (pointer: coarse)'
    ).matches;
    if (isTouchPrimary) return;

    document.documentElement.classList.add('js-smooth-scroll');

    let scrollY = window.scrollY;
    let targetY = scrollY;
    let velocity = 0;
    let isRunning = false;

    const ease = 0.08;
    const friction = 0.91;
    const wheelGain = 1;
    const momentumGain = 0.28;
    const stopThreshold = 0.02;

    function maxScrollY() {
      return Math.max(
        0,
        document.documentElement.scrollHeight - window.innerHeight
      );
    }

    function clampY(y) {
      return Math.max(0, Math.min(y, maxScrollY()));
    }

    function normalizeWheelDelta(e) {
      let delta = e.deltaY;
      if (e.deltaMode === 1) {
        delta *= 32;
      } else if (e.deltaMode === 2) {
        delta *= window.innerHeight * 0.9;
      }
      return delta;
    }

    function syncFromNative() {
      scrollY = targetY = window.scrollY;
      velocity = 0;
    }

    function run() {
      isRunning = true;

      const diff = targetY - scrollY;
      scrollY += diff * ease + velocity;
      velocity *= friction;

      const max = maxScrollY();
      if (scrollY < 0) {
        scrollY = 0;
        velocity *= 0.55;
        targetY = Math.max(0, targetY);
      } else if (scrollY > max) {
        scrollY = max;
        velocity *= 0.55;
        targetY = Math.min(max, targetY);
      }

      window.scrollTo(0, scrollY);

      const stillMoving =
        Math.abs(velocity) > stopThreshold ||
        Math.abs(targetY - scrollY) > stopThreshold;

      if (stillMoving) {
        requestAnimationFrame(run);
      } else {
        scrollY = targetY = clampY(scrollY);
        velocity = 0;
        window.scrollTo(0, scrollY);
        isRunning = false;
      }
    }

    function kick() {
      if (!isRunning) requestAnimationFrame(run);
    }

    function onWheel(e) {
      if (e.ctrlKey || e.metaKey) return;

      /* 左右分割パネルなど、要素内スクロールはネイティブに任せる */
      if (
        e.target.closest(
          '.article-layout__visual, .article-layout__body, .lightbox, .sidebar'
        )
      ) {
        return;
      }

      e.preventDefault();

      if (Math.abs(window.scrollY - scrollY) > 2) syncFromNative();

      const delta = normalizeWheelDelta(e) * wheelGain;
      targetY = clampY(targetY + delta);
      velocity += delta * momentumGain;
      kick();
    }

    window.addEventListener('wheel', onWheel, { passive: false, capture: true });

    document.querySelectorAll('a[href^="#"]').forEach((link) => {
      link.addEventListener('click', (e) => {
        const id = link.getAttribute('href');
        if (!id || id === '#') return;

        const el = document.querySelector(id);
        if (!el) return;

        e.preventDefault();
        targetY = clampY(el.getBoundingClientRect().top + window.scrollY);
        velocity = 0;
        kick();
      });
    });

    window.addEventListener('resize', () => {
      targetY = clampY(targetY);
      scrollY = clampY(scrollY);
    });
  }

  /* ---------- ライトボックス（画像拡大表示） ---------- */
  function initLightbox() {
    const galleries = document.querySelectorAll('[data-lightbox]');
    if (!galleries.length) return;

    const lightbox = document.createElement('div');
    lightbox.className = 'lightbox';
    lightbox.hidden = true;
    lightbox.setAttribute('role', 'dialog');
    lightbox.setAttribute('aria-modal', 'true');
    lightbox.setAttribute('aria-label', '画像拡大表示');
    lightbox.innerHTML = `
      <div class="lightbox__overlay" data-lightbox-close></div>
      <button type="button" class="lightbox__close" data-lightbox-close aria-label="閉じる">&times;</button>
      <button type="button" class="lightbox__nav lightbox__nav--prev" aria-label="前の画像">&#8592;</button>
      <button type="button" class="lightbox__nav lightbox__nav--next" aria-label="次の画像">&#8594;</button>
      <figure class="lightbox__figure">
        <img class="lightbox__image" src="" alt="">
      </figure>
    `;
    document.body.appendChild(lightbox);

    const image = lightbox.querySelector('.lightbox__image');
    const prevBtn = lightbox.querySelector('.lightbox__nav--prev');
    const nextBtn = lightbox.querySelector('.lightbox__nav--next');
    let lastFocused = null;
    let slides = [];
    let currentIndex = 0;

    function getImageSrc(img) {
      return img.currentSrc || img.src;
    }

    function showAt(index) {
      if (!slides.length) return;
      currentIndex = (index + slides.length) % slides.length;
      const slide = slides[currentIndex];
      image.src = getImageSrc(slide);
      image.alt = slide.alt || '';
      const hasMultiple = slides.length > 1;
      prevBtn.hidden = !hasMultiple;
      nextBtn.hidden = !hasMultiple;
    }

    function open(gallery, clickedImg) {
      slides = Array.from(gallery.querySelectorAll('img'));
      currentIndex = Math.max(0, slides.indexOf(clickedImg));
      lastFocused = document.activeElement;
      showAt(currentIndex);
      lightbox.hidden = false;
      document.body.classList.add('is-lightbox-open');
      lightbox.querySelector('.lightbox__close').focus();
    }

    function close() {
      lightbox.hidden = true;
      image.removeAttribute('src');
      slides = [];
      currentIndex = 0;
      document.body.classList.remove('is-lightbox-open');
      if (lastFocused && typeof lastFocused.focus === 'function') {
        lastFocused.focus();
      }
    }

    function goPrev() {
      showAt(currentIndex - 1);
    }

    function goNext() {
      showAt(currentIndex + 1);
    }

    galleries.forEach((gallery) => {
      gallery.addEventListener('click', (e) => {
        const thumb = e.target.closest('img');
        if (!thumb || !gallery.contains(thumb)) return;
        e.preventDefault();
        open(gallery, thumb);
      });
    });

    prevBtn.addEventListener('click', (e) => {
      e.stopPropagation();
      goPrev();
    });

    nextBtn.addEventListener('click', (e) => {
      e.stopPropagation();
      goNext();
    });

    lightbox.addEventListener('click', (e) => {
      if (e.target.closest('[data-lightbox-close]')) close();
    });

    document.addEventListener('keydown', (e) => {
      if (lightbox.hidden) return;
      if (e.key === 'Escape') close();
      if (slides.length < 2) return;
      if (e.key === 'ArrowLeft') goPrev();
      if (e.key === 'ArrowRight') goNext();
    });
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

  /* ---------- フッター：パス表示 ---------- */
  function initFooterPath() {
    const footer = document.querySelector('.footer');
    if (!footer || footer.querySelector('.footer-path')) return;

    const ROOT_LABEL = 'HOME';
    const isWorks = /\/works\//.test(window.location.pathname) ||
      /\\works\\/.test(window.location.pathname);

    function getSitePathSegments() {
      let path = decodeURIComponent(window.location.pathname);
      const marker = '302_Homepage';
      const markerIndex = path.indexOf(marker);
      if (markerIndex !== -1) {
        path = path.slice(markerIndex + marker.length);
      }
      path = path.replace(/^[/\\]+/, '');
      if (!path) return [];
      return path.split(/[/\\]/).filter(Boolean);
    }

    function hrefForSegments(targetSegments) {
      if (targetSegments.length === 0) {
        return isWorks ? '../index.html' : 'index.html';
      }

      if (targetSegments.length === 1 && targetSegments[0] === 'works') {
        return isWorks ? '../index.html#works' : 'index.html#works';
      }

      const current = getSitePathSegments();
      let common = 0;
      const compareLen = Math.min(
        current.length - 1,
        targetSegments.length
      );
      while (
        common < compareLen &&
        current[common] === targetSegments[common]
      ) {
        common += 1;
      }

      const ups = Math.max(0, current.length - 1 - common);
      const down = targetSegments.slice(common);
      return `${'../'.repeat(ups)}${down.join('/')}`;
    }

    const segments = getSitePathSegments();
    const nav = document.createElement('nav');
    nav.className = 'footer-path';
    nav.setAttribute('aria-label', '現在のパス');

    const rootLink = document.createElement('a');
    rootLink.href = hrefForSegments([]);
    rootLink.textContent = ROOT_LABEL;
    nav.appendChild(rootLink);

    segments.forEach((segment, index) => {
      const sep = document.createElement('span');
      sep.className = 'footer-path__sep';
      sep.setAttribute('aria-hidden', 'true');
      sep.textContent = '\\';
      nav.appendChild(sep);

      const isLast = index === segments.length - 1;
      const targetSegments = segments.slice(0, index + 1);

      if (isLast) {
        const current = document.createElement('span');
        current.className = 'footer-path__current';
        current.textContent = segment;
        nav.appendChild(current);
      } else {
        const link = document.createElement('a');
        link.href = hrefForSegments(targetSegments);
        link.textContent = segment;
        nav.appendChild(link);
      }
    });

    const content = footer.querySelector('.footer__content');
    if (content) {
      footer.insertBefore(nav, content);
    } else {
      footer.prepend(nav);
    }
  }

  /* ---------- 左右分割ページ（高さをヘッダー／フッターに合わせる） ---------- */
  function initArticleSplit() {
    const split = document.querySelector('.article--split');
    if (!split) return;

    const desktopQuery = window.matchMedia('(min-width: 901px)');

    function updateChromeMetrics() {
      const nav = document.querySelector('.nav');
      const footer = document.querySelector('.footer');
      if (nav) {
        document.documentElement.style.setProperty(
          '--nav-height',
          `${nav.offsetHeight}px`
        );
      }
      if (footer) {
        document.documentElement.style.setProperty(
          '--footer-height',
          `${footer.offsetHeight}px`
        );
      }
    }

    function setSplitMode() {
      updateChromeMetrics();
      document.body.classList.toggle('is-article-split', desktopQuery.matches);
    }

    setSplitMode();
    desktopQuery.addEventListener('change', setSplitMode);
    window.addEventListener('resize', setSplitMode);
  }

  /* ---------- 作品リスト：並び替え ---------- */
  function initWorksListSort() {
    const list = document.querySelector('.works-list');
    if (!list) return;

    const buttons = list.querySelectorAll('.works-list__sort');
    const columnMap = {
      file: 'file',
      title: 'title',
      year: 'year',
      material: 'material',
    };

    let activeKey = null;
    let direction = 1;

    function getRows() {
      return [...list.querySelectorAll('.works-list__row')];
    }

    function getCellValue(row, key) {
      const col = row.querySelector(`.works-list__col--${columnMap[key]}`);
      return col ? col.textContent.trim() : '';
    }

    function compareRows(a, b, key) {
      const aValue = getCellValue(a, key);
      const bValue = getCellValue(b, key);

      if (key === 'year') {
        const aYear = parseInt(aValue.match(/\d+/)?.[0] ?? '0', 10);
        const bYear = parseInt(bValue.match(/\d+/)?.[0] ?? '0', 10);
        return aYear - bYear;
      }

      return aValue.localeCompare(bValue, 'ja');
    }

    function updateButtons(key) {
      buttons.forEach((button) => {
        const isActive = button.dataset.sort === key;
        button.classList.toggle('is-active', isActive);
        button.setAttribute(
          'aria-sort',
          isActive ? (direction === 1 ? 'ascending' : 'descending') : 'none'
        );
      });
    }

    function sortBy(key) {
      if (activeKey === key) {
        direction *= -1;
      } else {
        activeKey = key;
        direction = 1;
      }

      const sortedRows = getRows().sort(
        (a, b) => compareRows(a, b, key) * direction
      );

      sortedRows.forEach((row) => list.appendChild(row));
      updateButtons(key);
    }

    buttons.forEach((button) => {
      button.addEventListener('click', () => sortBy(button.dataset.sort));
    });

    sortBy('year');
  }

  /* ---------- ヒーローロゴ：ゆらゆら ---------- */
  function initHeroLogoFloat() {
    const logo = document.querySelector('.hero-logo');
    if (!logo || prefersReducedMotion) return;
    logo.classList.add('hero-logo--float');
  }

  function boot() {
    document.documentElement.classList.add('js-enabled');
    initInertialScroll();
    initScrollReveal();
    initNavScroll();
    initLightbox();
    initArticleSplit();
    initFooterPath();
    initWorksListSort();
    initHeroLogoFloat();
    if (!prefersReducedMotion) initHeroCanvas();
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', boot);
  } else {
    boot();
  }
})();
