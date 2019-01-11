from __future__ import print_function
import tensorflow as tf
from tensorflow.python.client import timeline

import numpy as np
import pytest


rows = 1000
cols = 1500


def test_relu():
    with tf.Graph().as_default():
        with tf.device('/gpu:0'):
            tf_a = tf.placeholder(tf.float32, [rows, cols], 'a')
            tf_out = tf.nn.relu(tf_a)

            a = np.random.randn(rows, cols).astype(np.float32)
            with tf.Session(config=tf.ConfigProto(log_device_placement=False)) as sess:
                run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
                run_metadata = tf.RunMetadata()

                out = sess.run(tf_out, {tf_a: a}, options=run_options, run_metadata=run_metadata)
            print('a', a)
            print('out', out)
            # diff = np.abs(gpu_out - expected).max()
            # print('diff', diff)

            tl = timeline.Timeline(run_metadata.step_stats)
            ctf = tl.generate_chrome_trace_format()
            with open('{}_timeline.json'.format("relu"), 'w') as f:
                f.write(ctf)
            # assert diff <= 1e-4
