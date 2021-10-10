#!/usr/bin/env python3
from matplotlib import pyplot as plt
from numpy import array, concatenate, newaxis

naive1=array([[0.000152556, 0.125935],
[0.000450608, 0.291193],
[0.00163661, 1.18562],
[0.399471, 434.123]])
naive2=array([[4.63664e-08, 6.86135e-06],
[0.000489405, 0.386948],
[0.0448181, 35467.2],
[211.194, 1.73584e+09]])
naive3=array([[0.000499782, 0.360073],
[0.000379185, 0.359349],
[0.000490592, 0.363305]])

twopass1=array([[0.000152556, 2.63553e-06],
[0.000450608, 6.47235e-06],
[0.00163661, 8.56111e-05],
[0.399471, 0.157957]])
twopass2=array([[4.63664e-08, 6.24896e-06],
[0.000489405, 7.89399e-06],
[0.0448181, 0.00259575],
[211.194, 44603.5]])
twopass3=array([[0.000499782, 9.36864e-06],
[0.000379185, 0.000697008],
[0.000490592, 0.0782981]])

shifted1=array([[9.36828e-06, 9.58744e-06],
[1.04124e-05, 2.23489e-05],
[1.43969e-05, 0.00014482],
[3.60311e-05, 0.00389937]])
shifted2=array([[3.86267e-06, 2.74033e-05],
[9.59891e-06, 3.87046e-05],
[0.000724609, 5.17194e-05],
[0.0252909, 0.000287744]])
shifted3=array([[1.06467e-05, 2.6365e-05],
[4.29749e-05, 0.00316962],
[0.000352583, 0.262978]])

welford1=array([[0.000128701, 1.61338e-05],
[0.000431541, 1.94118e-05],
[0.00149906, 8.60965e-05],
[0.00408751, 0.00254016]])
welford2=array([[5.00365e-08, 6.54427e-06],
[0.00057032, 2.25163e-05],
[0.0446786, 0.00249691],
[0.469618, 0.249521]])
welford3=array([[0.000532903, 1.86705e-05],
[0.000469934, 0.000755561],
[0.000528572, 0.076777]])


naiveb=[0.605, 0.618, 0.095, 0.179, 0.360, 0.120, 0.222, 0.421]
twopassb=[1.38, 1.45, 0.152, 0.298, 0.582, 0.281, 0.547, 1.08]
shiftedb=[0.795, 0.852, 0.119, 0.229, 0.449, 0.163, 0.334, 0.614]
welfordb=[1.54, 1.72, 0.353, 0.702, 1.38, 0.355, 0.696, 1.37]


plt.figure()
plt.title("Varying number of points - error of computed mean (lower is better)")
plt.bar([0.7, 1.7, 2.7, 3.7], naive1[:, 0], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9, 3.9], twopass1[:, 0], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1, 4.1], shifted1[:, 0], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3, 4.3], welford1[:, 0], width=0.2, label="welford")
plt.gca().set_yscale('log')
plt.gca().set_xticks([1, 2, 3, 4])
plt.gca().set_xticklabels(('1K', '10K', '100K', '1M'))
#plt.gca().set_ylim([0.0, 0.02])
plt.legend()
plt.savefig("1.svg")
# plt.show()

plt.figure()
plt.title("Varying number of points - error of computed covariance (lower is better)")
plt.bar([0.7, 1.7, 2.7, 3.7], naive1[:, 1], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9, 3.9], twopass1[:, 1], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1, 4.1], shifted1[:, 1], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3, 4.3], welford1[:, 1], width=0.2, label="welford")
plt.gca().set_yscale('log')
plt.gca().set_xticks([1, 2, 3, 4])
plt.gca().set_xticklabels(('1K', '10K', '100K', '1M'))
#plt.gca().set_ylim([0.0, 2.0])
plt.legend()
plt.savefig("2.svg")
# plt.show()

plt.figure()
plt.title("Varying distance of cloud from origin - error of computed mean (lower is better)")
plt.bar([0.7, 1.7, 2.7, 3.7], naive2[:, 0], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9, 3.9], twopass2[:, 0], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1, 4.1], shifted2[:, 0], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3, 4.3], welford2[:, 0], width=0.2, label="welford")
plt.gca().set_yscale('log')
plt.gca().set_xticks([1, 2, 3, 4])
plt.gca().set_xticklabels(('at origin', 'near origin', 'medium', 'far from origin'))
#plt.gca().set_ylim([0.0, 1.0])
plt.legend()
plt.savefig("3.svg")
# plt.show()

plt.figure()
plt.title("Varying distance of cloud from origin - error of computed covariance (lower is better)")
plt.bar([0.7, 1.7, 2.7, 3.7], naive2[:, 1], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9, 3.9], twopass2[:, 1], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1, 4.1], shifted2[:, 1], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3, 4.3], welford2[:, 1], width=0.2, label="welford")
plt.gca().set_yscale('log')
plt.gca().set_xticks([1, 2, 3, 4])
plt.gca().set_xticklabels(('at origin', 'near origin', 'medium', 'far from origin'))
#plt.gca().set_ylim([0.0, 0.5])
plt.legend()
plt.savefig("4.svg")
# plt.show()

plt.figure()
plt.title("Varying covariance of cloud - error of computed mean (lower is better)")
plt.bar([0.7, 1.7, 2.7], naive3[:, 0], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9], twopass3[:, 0], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1], shifted3[:, 0], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3], welford3[:, 0], width=0.2, label="welford")
plt.gca().set_yscale('log')
plt.gca().set_xticks([1, 2, 3])
plt.gca().set_xticklabels(('low', 'medium', 'high'))
plt.legend()
plt.savefig("5.svg")
# plt.show()

plt.figure()
plt.title("Varying covariance of cloud - error of computed covariance (lower is better)")
plt.bar([0.7, 1.7, 2.7], naive3[:, 1], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9], twopass3[:, 1], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1], shifted3[:, 1], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3], welford3[:, 1], width=0.2, label="welford")
plt.gca().set_yscale('log')
plt.gca().set_xticks([1, 2, 3])
plt.gca().set_xticklabels(('low', 'medium', 'high'))
plt.legend()
plt.savefig("6.svg")
# plt.show()

plt.figure()
plt.title("benchmark (lower is better)")
plt.bar([0.7, 1.7], naiveb[0:2], width=0.2, label="naive")
plt.bar([0.9, 1.9], twopassb[0:2], width=0.2, label="twopass")
plt.bar([1.1, 2.1], shiftedb[0:2], width=0.2, label="shifted")
plt.bar([1.3, 2.3], welfordb[0:2], width=0.2, label="welford")
plt.gca().set_xticks([1, 2])
plt.gca().set_xticklabels(('mug', 'milk'))
plt.legend()
plt.savefig("7.svg")
# plt.show()

plt.figure()
plt.title("benchmark (lower is better)")
plt.bar([0.7, 1.7, 2.7], naiveb[2:5], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9], twopassb[2:5], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1], shiftedb[2:5], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3], welfordb[2:5], width=0.2, label="welford")
plt.gca().set_ylim([0.0, 1.5])
plt.gca().set_xticks([1, 2, 3])
plt.gca().set_xticklabels(('50', '100', '200'))
plt.legend()
plt.savefig("8.svg")
# plt.show()

plt.figure()
plt.title("benchmark (lower is better)")
plt.bar([0.7, 1.7, 2.7], naiveb[5:8], width=0.2, label="naive")
plt.bar([0.9, 1.9, 2.9], twopassb[5:8], width=0.2, label="twopass")
plt.bar([1.1, 2.1, 3.1], shiftedb[5:8], width=0.2, label="shifted")
plt.bar([1.3, 2.3, 3.3], welfordb[5:8], width=0.2, label="welford")
plt.gca().set_ylim([0.0, 1.5])
plt.gca().set_xticks([1, 2, 3])
plt.gca().set_xticklabels(('50', '100', '200'))
plt.legend()
plt.savefig("9.svg")
# plt.show()
