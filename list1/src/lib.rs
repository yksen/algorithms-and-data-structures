mod exercise1 {
    pub fn has_divisor(n: i32) -> bool {
        for i in 2..n {
            if n % i == 0 {
                return true;
            }
        }
        false
    }
}

mod exercise2 {
    pub fn has_divisor(n: i32) -> bool {
        let mut i = 2;
        while i * i <= n {
            if n % i == 0 {
                return true;
            }
            i += 1;
        }
        false
    }
}

mod exercise3 {}

mod exercise4 {
    pub fn binary_search_root(f: impl Fn(f32) -> f32) -> f32 {
        0.0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_has_divisor() {
        assert_eq!(exercise1::has_divisor(1), false);
        assert_eq!(exercise1::has_divisor(2), false);
        assert_eq!(exercise1::has_divisor(3), false);
        assert_eq!(exercise1::has_divisor(4), true);
        assert_eq!(exercise1::has_divisor(5), false);

        assert_eq!(exercise2::has_divisor(3), false);
        assert_eq!(exercise2::has_divisor(4), true);
    }

    #[test]
    fn test_binary_search_root() {
        // assert_eq!(exercise4::binary_search_root(|x: f32| x - 0.5), 0.5);
    }
}
