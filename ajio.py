from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.common.exceptions import NoSuchElementException        

def check_exists_by_xpath(browser,xpath):
    try:
        browser.find_element_by_xpath(xpath)
    except NoSuchElementException:
        return False
    return True

def scrollDown(browser, numberOfScrollDowns):
 body = browser.find_element_by_tag_name("body")
 while numberOfScrollDowns >=0:
  body.send_keys(Keys.PAGE_DOWN)
  numberOfScrollDowns -= 1
 return browser

browser = webdriver.Firefox()
browser.get("https://www.ajio.com/s/women-fashion-jewellery")
browser = scrollDown(browser, 500)
"""b = check_exists_by_xpath(browser,"//*[contains(text(), 'Show more products')]")
if b :
	a = browser.find_element_by_xpath("//*[contains(text(), 'Show more products')]")
	browser = scrollDown(browser, 100)
while b:
	print "hello"
	a.click()
	browser = scrollDown(browser, 100)
	b = check_exists_by_xpath(browser,"//*[contains(text(), 'Show more products')]")
	if b:
		a = browser.find_element_by_xpath("//*[contains(text(), 'Show more products')]")
"""
all_hover_elements = browser.find_elements_by_xpath("//div[@id='showAllProducts']//a[@id='fnl-plp-producthov']")
print len(all_hover_elements)
