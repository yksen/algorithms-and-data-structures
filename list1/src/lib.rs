pub mod ex1 {
    pub fn has_divisor_under_n(n: u32) -> bool {
        for i in 2..n {
            if n % i == 0 {
                return true;
            }
        }
        false
    }

    pub fn has_divisor_under_square_root_of_n(n: u32) -> bool {
        let mut i = 2;
        while i * i <= n {
            if n % i == 0 {
                return true;
            }
            i += 1;
        }
        false
    }

    pub fn erastotenes_sieve(n: usize) -> Vec<usize> {
        let mut sieve = vec![false; n];
        for i in 2..n {
            if sieve[i] == false {
                let mut j = 2 * i;
                while j < n {
                    sieve[j] = true;
                    j += i;
                }
            }
        }
        sieve[0..=1].iter_mut().for_each(|x| *x = true);
        sieve
            .iter()
            .enumerate()
            .filter(|(_, &x)| !x)
            .map(|(i, _)| i)
            .collect()
    }
}

pub mod ex3 {
    pub fn recursive_power(base: f32, exponent: u32) -> f32 {
        if exponent == 1 {
            base
        } else if exponent % 2 == 0 {
            let result = recursive_power(base, exponent / 2);
            result * result
        } else {
            recursive_power(base, exponent - 1) * base
        }
    }

    pub fn power(base: f32, exponent: u32) -> f32 {
        let mut x = base;
        let mut y = 1.0;
        let mut n = exponent;
        while n > 1 {
            if n % 2 == 0 {
                x *= x;
                n /= 2;
            } else {
                y *= x;
                x *= x;
                n = (n - 1) / 2;
            }
        }
        x * y
    }
}

pub mod ex4 {
    pub fn binary_search_root(f: impl Fn(f32) -> f32) -> f32 {
        let mut left = 0.0;
        let mut right = 1.0;
        let mut middle = (left + right) / 2.0;
        while middle != left && middle != right {
            if f(middle) > 0.0 {
                right = middle;
            } else {
                left = middle;
            }
            middle = (left + right) / 2.0;
        }
        middle
    }
}

pub mod ex6 {
    pub struct Node {
        pub value: i32,
        pub next: Option<Box<Node>>,
    }

    impl Node {
        pub fn new(value: i32) -> Self {
            Self { value, next: None }
        }

        pub fn print(&self) {
            print!("{} ", self.value);
            if let Some(next) = &self.next {
                next.print();
            }
        }

        pub fn sum(&self) -> i32 {
            self.value + self.next.as_ref().map_or(0, |x| x.sum())
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_has_divisor_under_n() {
        assert_eq!(ex1::has_divisor_under_n(1), false);
        assert_eq!(ex1::has_divisor_under_n(2), false);
        assert_eq!(ex1::has_divisor_under_n(3), false);
        assert_eq!(ex1::has_divisor_under_n(4), true);
        assert_eq!(ex1::has_divisor_under_n(5), false);
    }

    #[test]
    fn test_has_divisor_under_square_root_of_n() {
        assert_eq!(ex1::has_divisor_under_square_root_of_n(3), false);
        assert_eq!(ex1::has_divisor_under_square_root_of_n(4), true);
    }

    #[test]
    fn test_erastotenes_sieve() {
        assert_eq!(ex1::erastotenes_sieve(10), vec![2, 3, 5, 7]);
        assert_eq!(ex1::erastotenes_sieve(20), vec![2, 3, 5, 7, 11, 13, 17, 19]);
    }

    #[test]
    fn test_recursive_power() {
        assert_eq!(ex3::recursive_power(2.0, 1), 2.0);
        assert_eq!(ex3::recursive_power(2.0, 2), 4.0);
        assert_eq!(ex3::recursive_power(2.0, 3), 8.0);
        assert_eq!(ex3::recursive_power(2.0, 4), 16.0);
        assert_eq!(ex3::recursive_power(2.0, 5), 32.0);
        assert_eq!(ex3::recursive_power(2.0, 15), 32768.0);
        assert_eq!(ex3::recursive_power(2.0, 16), 65536.0);
    }

    #[test]
    fn test_power() {
        assert_eq!(ex3::power(2.0, 1), 2.0);
        assert_eq!(ex3::power(2.0, 2), 4.0);
        assert_eq!(ex3::power(2.0, 3), 8.0);
        assert_eq!(ex3::power(2.0, 4), 16.0);
        assert_eq!(ex3::power(2.0, 5), 32.0);
        assert_eq!(ex3::power(2.0, 7), 128.0);
        assert_eq!(ex3::power(2.0, 8), 256.0);
        assert_eq!(ex3::power(2.0, 15), 32768.0);
        assert_eq!(ex3::power(2.0, 16), 65536.0);
    }

    #[test]
    fn test_binary_search_root() {
        assert_eq!(ex4::binary_search_root(|x| x - 0.5), 0.5);
        assert_eq!(ex4::binary_search_root(|x| x * x - 0.25), 0.5);
        assert_eq!(ex4::binary_search_root(f32::sin), 0.0);
    }

    #[test]
    fn test_node_sum() {
        let mut node = ex6::Node::new(1);
        node.next = Some(Box::new(ex6::Node::new(2)));
        node.next.as_mut().unwrap().next = Some(Box::new(ex6::Node::new(3)));
        assert_eq!(node.sum(), 6);
    }
}
