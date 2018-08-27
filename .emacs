
;; Added by Package.el.  This must come before configurations of
;; installed packages.  Don't delete this line.  If you don't want it,
;; just comment it out by adding a semicolon to the start of the line.
;; You may delete these explanatory comments.
(package-initialize)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(custom-enabled-themes (quote (tango-dark)))
 '(package-selected-packages
   (quote
    (magit elpy workgroups nyan-mode yasnippet-snippets ace-window yasnippet ace-jump-mode iy-go-to-char company-irony company-irony-c-headers flycheck-irony irony))))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

(require 'package)

(add-to-list
   'package-archives
   ;; '("melpa" . "http://stable.melpa.org/packages/") ; many packages won't show if using stable
   '("melpa" . "http://melpa.milkbox.net/packages/")
   t)

(add-hook 'c++-mode-hook 'irony-mode)
(add-hook 'c-mode-hook 'irony-mode)
(add-hook 'objc-mode-hook 'irony-mode)

(add-hook 'irony-mode-hook 'irony-cdb-autosetup-compile-options)
(require 'iy-go-to-char)
(define-key global-map (kbd "C-c SPC") 'ace-jump-mode)
(global-set-key (kbd "C-c f") 'iy-go-to-char)
(global-set-key (kbd "C-c F") 'iy-go-to-char-backward)
(global-set-key (kbd "C-c ;") 'iy-go-to-or-up-to-continue)
(global-set-key (kbd "C-c ,") 'iy-go-to-or-up-to-continue-backward)
(require 'yasnippet)
(yas-global-mode 1)
(setq-default c-basic-offset 4)
(require 'ace-window)
(global-set-key (kbd "S-C-<left>") 'shrink-window-horizontally)
(global-set-key (kbd "S-C-<right>") 'enlarge-window-horizontally)
(global-set-key (kbd "S-C-<down>") 'shrink-window)
(global-set-key (kbd "S-C-<up>") 'enlarge-window)
(global-set-key (kbd "C-x o") 'ace-window)

(add-to-list 'load-path "/home/sdl/.emacs.d/my-shell/")
(require 'my-shell)

(defun kek-shell()
  (interactive)
  (switch-to-buffer (shell-get-buffer-create))
  )

(global-set-key (kbd "C-c t") 'kek-shell)

(require 'workgroups)
(setq wg-prefix-key (kbd "C-c w"))
(workgroups-mode 1)
(auto-revert-mode 1)
(setq auto-revert-interval 0.5)
(tool-bar-mode -1)
(add-to-list 'default-frame-alist '(fullscreen . maximized))
(wg-load "/home/sdl/.emacs.d/workgroups/cpp")
(setq inhibit-startup-screen t)
(set-face-attribute 'default nil :height 120)

(defun move-line-up ()
  "Move up the current line."
  (interactive)
  (transpose-lines 1)
  (forward-line -2)
  (indent-according-to-mode))

(defun move-line-down ()
  "Move down the current line."
  (interactive)
  (forward-line 1)
  (transpose-lines 1)
  (forward-line -1)
  (indent-according-to-mode))

(global-set-key (kbd "C-M-p") 'move-line-up)
(global-set-key (kbd "C-M-n") 'move-line-down)

(setq tab-width 4)

(fset 'cpp-run
   (lambda (&optional arg) "Keyboard macro." (interactive "p") (kmacro-exec-ring-item (quote ([24 111 49 24 19 24 111 50 24 19 24 111 51 46 24 19 24 111 52 134217790 46 47 114 117 110 46 115 115 104 backspace backspace 104 return 24 111 51] 0 "%d")) arg)))
(global-set-key (kbd "<f5>") 'cpp-run)

(desktop-save-mode 1)
(add-to-list 'load-path "/home/sdl/.emacs.d/autopair")
(require 'autopair)
(autopair-global-mode)
(eval-after-load 'company
  '(add-to-list 'company-backends 'company-irony))
(setq indent-tabs-mode nil)
(display-line-numbers-mode 1)
(define-key key-translation-map (kbd "C-j") (kbd "RET"))

(require 'org)
(define-key global-map "\C-cl" 'org-store-link)
(define-key global-map "\C-ca" 'org-agenda)
(setq org-log-done t)
(setq org-format-latex-options (plist-put org-format-latex-options :scale 1.5))
