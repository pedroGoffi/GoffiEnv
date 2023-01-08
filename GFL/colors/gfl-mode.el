(defconst gfl-mode-syntax-table
  (with-syntax-table(copy-syntax-table)
  (modify-syntax-entry ?/ ". 124b")
  (modify-syntax-entry ?* ". 23")
  (modify-syntax-entry ?\n "> b")
  (modify-syntax-entry ?' "\"")
  (syntax-table))
  "Syntax table for `gfl-mode'.")

(eval-and-compile
  (defconst gfl-keywords
    '(
      "var" "const" "algoritmo" "estrutura" "enum" "uniao"
      "retorne" "parar" "se" "ouse" "senao" "enquanto"
      "faca" "switch" "como" "e" "ou" "incluir"
      )))

(defconst gfl-highlights
  `((, (regexp-opt gfl-keywords 'symbols) . font-lock-keyword-face)))

(define-derived-mode gfl-mode prog-mode "gfl"
  (setq font-lock-defaults '(gfl-highlights))
  (setq-local comment-start "// "))

(add-to-list 'auto-mode-alist '("\\.gfl\\'" . gfl-mode))

(provide 'gfl-mode)

