import os, cv2, easyocr, re
import numpy as np

test = [
    '154러7070',
    '265버9344',
    '123가4568',
    '21고7824',
    '33너2222'
]

reader = easyocr.Reader(['ko'], gpu = True)

def save_img(file, name, img_dir="Images"):
	nparr = np.fromstring(file.read(), np.uint8)
	img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
	if not os.path.isdir(img_dir):
		os.mkdir(img_dir)
	cv2.imwrite(os.path.join(img_dir,str(name)+".jpg"), img)

def read_img(board):
	result = None
	results = reader.readtext(f"Images/{board}.jpg")
	if results:
		pattern1 = r'\d{2}[가-힣]\s\d{4}'
		pattern2 = r'\d{3}[가-힣]\s\d{4}'

		text =str(results[0][1])

		match1 = re.search(pattern1, text)
		match2 = re.search(pattern2, text)

		if match1 or match2:
			if match1:
				matched_text = str(match1.group())
			else:
				matched_text = str(match2.group())
			# print(matched_text)
		else:
			print(text)
		try:
			matched_text = matched_text.replace(" ", "")
		except:
			pass
		if matched_text in test:
			result = 1
		else:
			result = 0
	print(matched_text)
	return result