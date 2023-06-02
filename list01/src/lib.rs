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
            if n % 2 == 1 {
                y *= x;
            }
            n /= 2;
            x *= x;
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
            if f(middle) == 0.0 {
                return middle;
            } else if f(middle) > 0.0 {
                right = middle;
            } else {
                left = middle;
            }
            middle = (left + right) / 2.0;
        }
        middle
    }
}

pub mod ex5 {
    pub fn calculate_horner(a: Vec<f32>, x: f32) -> f32 {
        let mut product = 0.0;
        for a in a.iter().rev() {
            product = a + x * product;
        }
        product
    }
}

pub mod ex6 {
    #[derive(Clone)]
    pub struct Node {
        pub value: i32,
        pub next: Option<Box<Node>>,
    }

    impl From<Vec<i32>> for Node {
        fn from(values: Vec<i32>) -> Self {
            if values.is_empty() {
                Node {
                    value: 0,
                    next: None,
                }
            } else {
                let mut head = Node::new(values[0]);
                for value in values.iter().skip(1) {
                    head.insert(*value);
                }
                head
            }
        }
    }

    impl Node {
        pub fn new(value: i32) -> Self {
            Self { value, next: None }
        }

        pub fn print(&self) -> usize {
            let mut count = 1;
            print!("{} ", self.value);
            if let Some(next) = &self.next {
                count += next.print();
            }
            count
        }

        pub fn sum(&self) -> i32 {
            self.value + self.next.as_ref().map_or(0, |x| x.sum())
        }

        pub fn nth(&self, n: usize) -> Option<i32> {
            if n == 0 {
                Some(self.value)
            } else {
                self.next.as_ref().and_then(|x| x.nth(n - 1))
            }
        }

        pub fn insert(&mut self, value: i32) {
            if let Some(next) = &mut self.next {
                next.insert(value);
            } else {
                self.next = Some(Box::new(Node::new(value)));
            }
        }

        pub fn insert_after_smaller(&mut self, value: i32) {
            if let Some(next) = &mut self.next {
                if next.value < value {
                    next.insert_after_smaller(value);
                } else {
                    let mut new_node = Node::new(value);
                    new_node.next = self.next.take();
                    self.next = Some(Box::new(new_node));
                }
            } else {
                self.next = Some(Box::new(Node::new(value)));
            }
        }

        pub fn remove(&mut self, value: i32) {
            if self.value == value {
                self.value = self.next.as_ref().map_or(0, |x| x.value);
                self.next = self.next.as_mut().and_then(|x| x.next.take());
            }
            if let Some(next) = &mut self.next {
                if next.value == value {
                    self.next = next.next.take();
                    self.remove(value);
                } else {
                    next.remove(value);
                }
            }
        }

        pub fn filter(&mut self, predicate: impl Fn(i32) -> bool) {
            if !predicate(self.value) {
                self.value = self.next.as_ref().map_or(0, |x| x.value);
                self.next = self.next.as_mut().and_then(|x| x.next.take());
            }
            if let Some(next) = &mut self.next {
                if !predicate(next.value) {
                    self.next = next.next.take();
                    self.filter(predicate);
                } else {
                    next.filter(predicate);
                }
            }
        }

        pub fn destroy(&mut self) {
            self.value = 0;
            self.next = None;
        }

        pub fn reverse(&mut self) {
            unimplemented!();
        }

        pub fn merge(&mut self, other: &mut Node) {
            unimplemented!();
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
        assert_eq!(
            ex4::binary_search_root(|x| x * x + x - 1.0),
            0.61803399085998
        );
        assert_eq!(ex4::binary_search_root(f32::sin), 0.0);
    }

    #[test]
    fn test_calculate_horner() {
        assert_eq!(ex5::calculate_horner(vec![1.0, 2.0, 3.0], 10.0), 321.0);
        assert_eq!(ex5::calculate_horner(vec![1.0, 2.0, 3.0], 0.0), 1.0);
        assert_eq!(
            ex5::calculate_horner(vec![4.0, 5.0, 3.0, 0.0, 0.0, 18.0], 10.0),
            1800354.0
        );
    }

    #[test]
    fn test_node_print() {
        let head = ex6::Node::from(vec![]);
        assert_eq!(head.print(), 1);

        let head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        assert_eq!(head.print(), 5);
    }

    #[test]
    fn test_node_sum() {
        let head = ex6::Node::new(1);
        assert_eq!(head.sum(), 1);

        let head = ex6::Node::from(vec![]);
        assert_eq!(head.sum(), 0);

        let head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        assert_eq!(head.sum(), 15);

        let head = ex6::Node::from(vec![1, 2, 3, 4, 5, 6]);
        assert_eq!(head.sum(), 21);
    }

    #[test]
    fn test_node_nth() {
        let head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        assert_eq!(head.nth(0), Some(1));
        assert_eq!(head.nth(1), Some(2));
        assert_eq!(head.nth(2), Some(3));
        assert_eq!(head.nth(3), Some(4));
        assert_eq!(head.nth(4), Some(5));
        assert_eq!(head.nth(5), None);
    }

    #[test]
    fn test_node_insert() {
        let mut head = ex6::Node::new(1);
        head.insert(2);
        head.insert(3);
        head.insert(4);
        head.insert(5);
        assert_eq!(head.sum(), 15);
    }

    #[test]
    fn test_node_insert_after_smaller() {
        let mut head = ex6::Node::from(vec![1, 2, 3, 5, 6]);
        head.insert_after_smaller(4);
        assert_eq!(head.sum(), 21);
        assert_eq!(head.print(), 6);

        head = ex6::Node::from(vec![1, 3]);
        head.insert_after_smaller(2);
        assert_eq!(head.print(), 3);
        assert_eq!(head.nth(1), Some(2));

        head = ex6::Node::from(vec![1]);
        head.insert_after_smaller(2);
        assert_eq!(head.print(), 2);
        assert_eq!(head.nth(1), Some(2));
    }

    #[test]
    fn test_node_remove() {
        let mut head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        head.remove(3);
        assert_eq!(head.print(), 4);
        assert_eq!(head.sum(), 12);

        head = ex6::Node::from(vec![7, 7, 7]);
        head.remove(7);
        assert_eq!(head.print(), 1);
        assert_eq!(head.sum(), 0);

        head = ex6::Node::from(vec![3, 2, 3, 1, 8, 3]);
        head.remove(3);
        assert_eq!(head.print(), 3);
        assert_eq!(head.sum(), 11);

        head = ex6::Node::new(1);
        head.remove(1);
        assert_eq!(head.value, 0);
    }

    #[test]
    fn test_node_filter() {
        let mut head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        head.filter(|x| x % 2 == 0);
        assert_eq!(head.print(), 2);
        assert_eq!(head.sum(), 6);

        head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        head.filter(|x| x % 2 == 1);
        assert_eq!(head.print(), 3);
        assert_eq!(head.sum(), 9);
    }

    #[test]
    fn test_node_destroy() {
        let mut head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        head.destroy();
        assert_eq!(head.print(), 1);
    }

    #[test]
    fn test_node_reverse() {
        let mut head = ex6::Node::from(vec![1, 2, 3, 4, 5]);
        head.reverse();
        assert_eq!(head.print(), 5);
        assert_eq!(head.nth(0), Some(5));
        assert_eq!(head.nth(1), Some(4));
        assert_eq!(head.nth(2), Some(3));
        assert_eq!(head.nth(3), Some(2));
        assert_eq!(head.nth(4), Some(1));
    }
}
