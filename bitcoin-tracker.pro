TEMPLATE = subdirs
CONFIG += ordered

#bitcoin_price_fetcher transactions_data_base 
SUBDIRS += main_window

#bitcoin_price_fetcher.file  = bitcoin_price_fetcher/base.pro
#transactions_data_base.file  = transactions_data_base/satellite_comparator.pro
main_window.file = main_window/bitcoin_biper_mainwindow.pro
