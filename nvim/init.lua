local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not (vim.uv or vim.loop).fs_stat(lazypath) then
  vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "https://github.com/folke/lazy.nvim.git",
    "--branch=stable", -- latest stable release
    lazypath,
  })
end
vim.opt.rtp:prepend(lazypath)
vim.g.mapleader = " " -- Make sure to set `mapleader` before lazy so your mappings are correct
vim.g.maplocalleader = "\\" -- Same for `maplocalleader`
vim.g.mapleader = " "					-- sets leader key
vim.g.netrw_banner = 0					-- gets rid of the annoying banner for netrw
vim.g.netrw_browse_split=4				-- open in prior window
vim.g.netrw_altv = 1					-- change from left splitting to right splitting
vim.g.netrw_liststyle=3					-- tree style view in netrw
--vim.cmd("let g:netrw_list_hide=netrw_gitignore#Hide()")
vim.opt.title = true					-- show title
vim.cmd('set path+=**')					-- search current directory recursively
vim.opt.syntax = "ON"
vim.opt.backup = false
vim.opt.compatible = false				-- turn off vi compatibility mode
vim.opt.number = true					-- turn on line numbers
vim.opt.relativenumber = true				-- turn on relative line numbers
vim.opt.mouse = 'a'						-- enable the mouse in all modes
vim.opt.ignorecase = true				-- enable case insensitive searching
vim.opt.smartcase = true				-- all searches are case insensitive unless there's a capital letter
vim.opt.hlsearch = false				-- disable all highlighted search results
vim.opt.incsearch = true				-- enable incremental searching
vim.opt.wrap = false						-- enable text wrapping
vim.opt.tabstop = 4						-- tabs=4spaces
vim.opt.foldmethod = "manual"
vim.opt.shiftwidth = 4
vim.opt.fileencoding = "utf-8"				-- encoding set to utf-8
vim.opt.pumheight = 10					-- number of items in popup menu
vim.opt.showtabline = 2					-- always show the tab line
-- vim.opt.laststatus = 2					-- always show statusline
vim.opt.signcolumn = "auto"
vim.opt.expandtab = false				-- expand tab 
vim.opt.smartindent = true
--vim.opt.showcmd = true
--vim.opt.cmdheight = 2
--vim.opt.showmode = true
vim.opt.scrolloff = 8					-- scroll page when cursor is 8 lines from top/bottom
vim.opt.sidescrolloff = 8				-- scroll page when cursor is 8 spaces from left/right
vim.opt.guifont = "monospace:h17"		
vim.opt.clipboard = unnamedplus
vim.opt.completeopt= { "menuone", "noselect" }
vim.opt.splitbelow = true				-- split go below
vim.opt.splitright = true				-- vertical split to the right
vim.opt.termguicolors = true			-- terminal gui colors
vim.cmd('filetype plugin on')			-- set filetype 
vim.cmd('set wildmenu')					-- enable wildmenu
require("lazy").setup({
  	"folke/which-key.nvim",
  	{ "folke/neoconf.nvim", cmd = "Neoconf" },
  	"folke/neodev.nvim",
  	{
    'nvim-lualine/lualine.nvim',
    dependencies = { 'nvim-tree/nvim-web-devicons' }
  	},
  	{"RRethy/base16-nvim"},
	{'junegunn/fzf.vim'},
	{"junegunn/fzf"},
	{'williamboman/mason.nvim'},
	{'williamboman/mason-lspconfig.nvim'},
	{'VonHeikemen/lsp-zero.nvim', branch = 'v3.x'},
	{'neovim/nvim-lspconfig'},
	{'hrsh7th/cmp-nvim-lsp'},
	{'hrsh7th/nvim-cmp'},
	{'L3MON4D3/LuaSnip'},
	{
		  "folke/trouble.nvim",
		  opts = {}, -- for default options, refer to the configuration section for custom setup.
		  cmd = "Trouble",
		  keys = {
			{
			  "<leader>xx",
			  "<cmd>Trouble diagnostics toggle<cr>",
			  desc = "Diagnostics (Trouble)",
			},
			{
			  "<leader>xX",
			  "<cmd>Trouble diagnostics toggle filter.buf=0<cr>",
			  desc = "Buffer Diagnostics (Trouble)",
			},
			{
			  "<leader>cs",
			  "<cmd>Trouble symbols toggle focus=false<cr>",
			  desc = "Symbols (Trouble)",
			},
			{
			  "<leader>cl",
			  "<cmd>Trouble lsp toggle focus=false win.position=right<cr>",
			  desc = "LSP Definitions / references / ... (Trouble)",
			},
			{
			  "<leader>xL",
			  "<cmd>Trouble loclist toggle<cr>",
			  desc = "Location List (Trouble)",
			},
			{
			  "<leader>xQ",
			  "<cmd>Trouble qflist toggle<cr>",
			  desc = "Quickfix List (Trouble)",
			},
		  },
		},
	{
    'AlexvZyl/nordic.nvim',
    lazy = false,
    priority = 1000,
    config = function()
        require 'nordic' .load()
    end
	},
		{
		  "hedyhli/outline.nvim",
		  config = function()
			-- Example mapping to toggle outline
			vim.keymap.set("n", "<leader>o", "<cmd>Outline<CR>",
			  { desc = "Toggle Outline" })

			require("outline").setup {
			  -- Your setup opts here (leave empty to use defaults)
			}
		  end,
		},
	{
	  "sho-87/kanagawa-paper.nvim",
	  lazy = false,
	  priority = 1000,
	  opts = {},
	},
})
require('lualine').setup()
vim.cmd('colo base16-chalk')
local isLspDiagnosticsVisible = true
vim.keymap.set("n", "<leader>lx", function()
    isLspDiagnosticsVisible = not isLspDiagnosticsVisible
    vim.diagnostic.config({
        virtual_text = isLspDiagnosticsVisible,
        underline = isLspDiagnosticsVisible
    }) end)
--vim.cmd('highlight Normal guibg=NONE ctermbg=NONE')
