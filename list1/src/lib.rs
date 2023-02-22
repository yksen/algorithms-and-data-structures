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
    fn test_binary_search_root() {
        assert_eq!(exercise4::binary_search_root(|x: f32| x - 0.5), 0.5);
    }
}
