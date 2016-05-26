Statistical Functions
=====================

General
-------

.. function:: average(x1; x2; ...)

    Computes the arithmetic average of the arguments (sum of the arguments divided by their number). The arguments must share the same dimension.

.. function:: geomean(x1; x2; ...)

    Computes the geometric mean of the arguments, defined by ``product(x1; x2; ...)^(1/n)`` where ``n`` is the number of arguments. All the arguments may each have a different dimension. The geometric mean is useful for comparing sets of quantities that are very different in order of magnitude and even possibly dimension.

.. function:: median(x1; x2; ...)

    Computes the median of the arguments, i.e. the value dividing the set of arguments into two evenly sized parts.

    First the set of arguments is sorted. If the number of arguments is odd, the element in the middle of the sorted list is returned. If the number of arguments is even, the arithmetic mean of the two central elements is returned.

.. function:: min(x1; x2; ...)

    Returns the minimum out of the supplied argument list. The arguments must be real and share the same dimension.

.. function:: max(x1; x2; ...)

    Returns the maximum out of the supplied argument list. The arguments must be real and share the same dimension.

.. function:: sum(x1; x2; ...)

    Computes the sum of all the given arguments. These must share the same dimension.

.. function:: product(x1; x2; ...)

    Computes the product of all the given arguments.

.. function:: variance(x1; x2; ...)

    Computes the population variance of the arguments. The variance is measure for the spreading of a set of numbers.

    The arguments must share the same dimension.

    .. note::

        This function computes the *population variance*, which assumes that all possible realizations are all given as arguments. A function to estimate the variance from a sample (*sample variance*) is not included with SpeedCrunch.

.. function:: stddev(x1; x2; ...)

    Computes the standard deviation of the given arguments. It is obtained by taking the square root of the :func:`variance <variance>` of its arguments.

.. function:: absdev(x1; x2; ...)

    Computes the average absolute deviation of the arguments from their mean. This means that, with ``x = average(x1; x2; ...)``, we have::

        absdev(x1; x2; ... ) = abs(x1 - x) + abs(x2 - x) + ...



.. _binomial-distribution:

Binomial Distribution
---------------------

The binomial distribution is described by the parameters ``N`` and ``p``. It gives the probability distribution of the number of successful trials, when the total number of trials is given by ``N``, and each test is successful with probability ``p``. Not that unlike the :ref:`hypergeometric-distribution`, the probability ``p`` remains the same for all draws. The binomial distribution can be thought of drawing *with* replacement, while the hypergeometric distribution describes drawing *without* replacement.

.. function:: binomcdf(x; N; p)

    Binomial cumulative distribution function.

    :param x: maximum number of successes, must be integer
    :param N: number of trials, must be a positive integer
    :param p: probability to succeed a single trial, *0 <= p <= 1*

    The function computes the probability, that, for ``N`` independent repetitions of a test successful with probability ``p`` each, the total number of successes is **less than or equal** to ``x``.

    Example: When tossing a fair coin 9 times, what is the probability that we find Heads at most 5 times?

    ::

        binomcdf(5; 9; 0.5)
        = 0.74609375


.. function:: binompmf(x; N; p)

    Binomial probability mass function.

    :param x: maximum number of successes, must be integer
    :param N: number of trials, must be a positive integer
    :param p: probability to succeed a single trial, *0 <= p <= 1*

    The function computes the probability, that, for ``N`` independent repetitions of a test, each successful with probability ``p``, the total number of successes is **exactly equal** to ``x``.

    Example: When tossing a fair coin 9 times, what is the probability that we find Heads exactly 5 times?

    ::

        binompmf(5; 9; 0.5)
        = 0.24609375

.. function:: binommean(N; p)

    Mean (expectation) value of the given binomial distribution.

    The function computes the expected number of successes when an experiment is performed ``N`` times, each successful independently with probability ``p``. The result will simply be given by *N * p*.


.. function:: binomvar(N; p)

    Computes the variance of the given binomial distribution function, equal to *N \* p \* (1-p)*.

.. function:: ncr(N; k)

    Computes the binomial coefficient, equal to the number of possibilities of how to select ``k`` elements from a set of size ``N``. The order of the ``k`` elements is of no importance, i.e. permutations of a subset are not counted as an additional choice.

    In SpeedCrunch the domain of :func:`ncr` is extended to all real numbers. The result is *1/((\ N + 1) \* B(\ k + 1, N - k + 1))*, where *B(a, b)* is the complete Beta function.

.. function:: npr(N; k)

    Computes the binomial coefficient, equal to the number of possibilities of how to select ``k`` elements from a set of size ``N``. The order of the ``k`` elements **is** important, i.e. permutations of a subset are counted as an additional choice.

    In SpeedCrunch, the domain of :func:`npr` is extended to all real numbers. The result is *Γ(\ N + 1)/Γ(\ k)*, where *Γ* is the gamma function; see :func:`gamma`.


.. _hypergeometric-distribution:

Hypergeometric Distribution
---------------------------

The hypergeometric distribution is described by the three parameters ``N``, ``K`` and  ``n``. It describes the probability distribution of the number of successes when drawing ``n`` samples from a finite population of size ``N``, containing exactly ``K`` successes. Unlike the :ref:`binomial-distribution`, the hypergeometric distribution describes drawing *without* replacement.

.. function:: hyperpmf(k; N; K; n)

    Hypergeometric probability mass function.

    :param k: desired number of successes, must be integer
    :param N: total size of the population, must be positive integer
    :param K: number of successes within the population, *0 <= K <= N*, must be integer
    :param n: number of draws, *0 <= n <= N*, must be integer

    The function computes the probability that for ``n`` draws without replacement from a population of size ``N`` and containing ``K`` successes, the number of successes drawn is *exactly* equal to ``k``.

    Example: An urn contains 50 marbles, 40 of which are white, the rest are black. We draw 15 marbles without replacement. What is the probability of drawing 8 white ones?

    ::

        hyperpmf(8; 50; 40; 15)
        = 0.00410007


.. function:: hypercdf(k; N; K; n)

    Hypergeometric cumulative distribution function.

    :param k: maximum desired number of successes, must be integer
    :param N: total size of the population, must be positive integer
    :param K: number of successes within the population, *0 <= K <= N*, must be integer
    :param n: number of draws, *0 <= n <= N*, must be integer

    The function computes the probability that for ``n`` draws without replacement from a population of size ``N`` and containing ``K`` successes, the number of successes drawn is *smaller than or equal* to ``k``.

    Example: An urn contains 50 marbles, 40 of which are white, the rest are black. We draw 15 marbles without replacement. What is the probability of drawing at most 8 white ones?

    ::

        hypercdf(8; 50; 40; 15)
        = 0.00449015



.. function:: hypermean(N; k; n)

    Expected value of the given hypergeometric distribution.

    :param N: total size of the population, must be positive integer
    :param K: number of successes within the population, *0 <= K <= N*, must be integer
    :param n: number of draws, *0 <= n <= N*, must be integer

    Example: An urn contains 50 marbles, 40 of which are white, the rest are black. We draw 15 marbles without replacement. How many white marbles do we expect to find in our drawn sample?

    ::

        hypermean(50; 40; 15)
        = 12

.. function:: hypervar(N; k; n)

    Variance of the given hypergeometric distribution.

    :param N: total size of the population, must be positive integer
    :param K: number of successes within the population, *0 <= K <= N*, must be integer
    :param n: number of draws, *0 <= n <= N*, must be integer

    Example: An urn contains 50 marbles, 40 of which are white, the rest are black. We draw 15 marbles without replacement. We estimate the standard deviation of the experiment::

        sqrt(hypervar(50; 40; 15))
        = 1.309

    This number is an estimate on by how many marbles our sample will deviate from the expectation value.


Poisson Distribution
--------------------

The Poisson distribution is characterized by only a single parameter, named ``mu``. It represents both the mean and the variance of the distribution. It describes the probability distribution of the number of events during a fixed period of time, when the average rate of events is known. The Poisson distribution requires the events to be independent. This is usually a good approximation when the rate is low compared to the total population size.

.. function:: poipmf(x; mu)

    Poisson probability mass function.

    :param x: Desired number of events, must be integer
    :param mu: Average number of events, must be positive

    Computes the probability to observe exactly ``x`` events.

    *Example 1*: In a manufacturing process with yield of 99%, what is the probability that 2 out of 10 manufactured products are failures?
    First, we note that according to the yield, the expected number of failures are given by::

        10 * (1-0.99)
        = 0.1

    Hence the answer to the problem is::

        poipmf(2; 0.1)
        = 0.0045

    *Example 2* ::An insurance company expects 10 claims over the period of one year. What is the probability that instead as many as 20 claims will be filed?

    ::

        poipmf(20; 10)
        = 0.001866

.. function:: poicdf(x; mu)

    Poisson cumulative distribution function.

    :param x: Desired number of events, must be integer
    :param mu: Average number of events, must be positive

    Computes the probability to observe ``x`` or fewer events.

    Example: An insurance company expects 10 claims over the period of one year. What is the probability that more than 12 claims will be filed?

    ::

        1 - poicdf(12; 10)
        = 0.208


.. function:: poimean(mu)

    Computes the expectation value of the given Poisson distribution. By definition, this value is equal to ``mu``.

.. function:: poivar(mu)

    Computes the variance of the given Poisson distribution. By definition, this value is equal to ``mu``.
