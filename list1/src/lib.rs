mod exercise1 {
    pub fn has_divisor_under_n(n: i32) -> bool {
        for i in 2..n {
            if n % i == 0 {
                return true;
            }
        }
        false
    }

    pub fn has_divisor_under_square_root_of_n(n: i32) -> bool {
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

mod exercise4 {
    pub fn binary_search_root(f: impl Fn(f32) -> f32) -> f32 {
        0.0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_has_divisor_under_n() {
        assert_eq!(exercise1::has_divisor_under_n(1), false);
        assert_eq!(exercise1::has_divisor_under_n(2), false);
        assert_eq!(exercise1::has_divisor_under_n(3), false);
        assert_eq!(exercise1::has_divisor_under_n(4), true);
        assert_eq!(exercise1::has_divisor_under_n(5), false);
    }

    #[test]
    fn test_has_divisor_under_square_root_of_n() {
        assert_eq!(exercise1::has_divisor_under_square_root_of_n(3), false);
        assert_eq!(exercise1::has_divisor_under_square_root_of_n(4), true);
    }

    #[test]
    fn test_erastotenes_sieve() {
        assert_eq!(exercise1::erastotenes_sieve(10), vec![2, 3, 5, 7]);
        assert_eq!(
            exercise1::erastotenes_sieve(20),
            vec![2, 3, 5, 7, 11, 13, 17, 19]
        );
    }

    #[test]
    fn test_binary_search_root() {
        // assert_eq!(exercise4::binary_search_root(|x: f32| x - 0.5), 0.5);
    }
}
