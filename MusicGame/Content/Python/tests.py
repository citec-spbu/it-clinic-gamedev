import unittest
import numpy as np
import os
from unittest.mock import patch, mock_open, MagicMock
from pathlib import Path

# Импортируем модули
import beatmark
import clone_trigger_from_csv as ctc

# --- 1. Тестирование обработки пиков ---
class TestBeatProcessing(unittest.TestCase):
    def test_prune_peaks_removes_close_beats(self):
        # Проверка удаления слишком близких меток
        times = np.array([0.1, 0.15, 0.5, 0.8, 0.85])
        expected = np.array([0.1, 0.5, 0.8])
        result = beatmark.prune_peaks(times, min_interval=0.3)
        np.testing.assert_array_equal(result, expected)

# --- 2. Тестирование граничных случаев (тишина) ---
class TestAlgorithmEdgeCases(unittest.TestCase):
    def test_silent_audio_returns_no_beats(self):
        # Проверка, что на тихом сигнале не создаются фантомные биты
        sr = 1000
        y = np.zeros(sr * 2) 
        times, energy, _ = beatmark.detect_beats_energy(y, sr)
        self.assertEqual(len(times), 0)

# --- 3. Тестирование чтения CSV ---
class TestCSVReader(unittest.TestCase):
    def test_read_csv_skips_garbage(self):
        # Проверка, что парсер игнорирует заголовки и пустые строки
        csv_content = "timestamp_s\n0.5\n\n1.2\ninvalid_data\n2.5"
        with patch("builtins.open", mock_open(read_data=csv_content)):
            times = ctc.read_csv_times("fake_path.csv")
            self.assertEqual(times, [0.5, 1.2, 2.5])

# --- 4. Тестирование загрузки аудио ---
class TestAudioLoading(unittest.TestCase):
    @patch("scipy.io.wavfile.read")
    @patch("pathlib.Path.exists")
    def test_stereo_to_mono_conversion(self, mock_exists, mock_wav_read):
        # Проверка корректного сведения стерео в моно
        mock_exists.return_value = True
        fake_data = np.array([[1000, -1000], [2000, -2000]], dtype='int16')
        mock_wav_read.return_value = (44100, fake_data)
        
        y, sr = beatmark.read_wav_mono("fake.wav")
        self.assertEqual(len(y.shape), 1)
        self.assertTrue(np.max(np.abs(y)) <= 1.0)

# --- 5. Тестирование сохранения результата ---
class TestFileIO(unittest.TestCase):
    @patch("pandas.DataFrame.to_csv")
    @patch("beatmark.read_wav_mono")
    def test_run_saves_csv(self, mock_read, mock_to_csv):
        # Проверка, что функция run инициирует сохранение CSV
        mock_read.return_value = (np.random.rand(1000).astype('float32'), 1000)
        
        beatmark.run("dummy.wav")
        
        # Проверяем, что pandas.to_csv был вызван хотя бы раз
        self.assertTrue(mock_to_csv.called)
        # Проверяем путь сохранения из beatmark.py
        args, _ = mock_to_csv.call_args
        self.assertIn("beats_energy.csv", str(args[0]))

# --- Функция запуска ---

def run_unreal_tests():
    suite = unittest.TestSuite()
    loader = unittest.TestLoader()
    
    # Собираем все тесты
    test_classes = [
        TestBeatProcessing, 
        TestAlgorithmEdgeCases, 
        TestCSVReader, 
        TestAudioLoading,
        TestFileIO
    ]
    
    for test_class in test_classes:
        suite.addTest(loader.loadTestsFromTestCase(test_class))
    
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    
    if result.wasSuccessful():
        print(f"UNIT TESTS: PASSED ({result.testsRun} tests)")
    else:
        print(f"UNIT TESTS: FAILED (Errors: {len(result.errors)}, Failures: {len(result.failures)})")

if __name__ == "__main__":
    run_unreal_tests()