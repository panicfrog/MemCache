//
//  MemCache-Swift.swift
//  MemCacheDemo
//
//  Created by 叶永平 on 2023/4/24.
//

import Foundation

final public class MemCache {
  let cache: UnsafeMutableRawPointer!
  public init() {
    self.cache = MemCache_new()
  }
  
  public func getString(forKey key: String) -> String? {
    let cKey = key.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    
    let success = MemCache_get_string(cache, cKey, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    return value
  }
  
  public func getInt32(forKey key: String) -> Int32? {
    let cKey = key.cString(using: .utf8)
    var value: Int32 = 0
    let success = MemCache_get_int(cache, cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  public func getDouble(forKey key: String) -> Double? {
    let cKey = key.cString(using: .utf8)
    var value: Double = 0
    let success = MemCache_get_double(cache, cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  public func getBool(forKey key: String) -> Bool? {
    let cKey = key.cString(using: .utf8)
    var value: Bool = true
    let success = MemCache_get_bool(cache, cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  public func put(value: Int32, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_int(cache, cKey, value)
  }
  
  public func put(value: Bool, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_bool(cache, cKey, value)
  }
  
  public func put(value: Double, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_double(cache, cKey, value)
  }
  
  public func put(value: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_put_string(cache, cKey, cValue)
  }
  
  public func putJson(value: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_put_json(cache, cKey, cValue)
  }
  
  public func getJson(forkey key: String) -> String? {
    let cKey = key.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    
    let success = MemCache_get_json(cache, cKey, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    return value
  }
  
  public func queryJson(forKey key: String, with path: String) -> String? {
    let cKey = key.cString(using: .utf8)
    let cPath = path.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    let success = MemCache_query_json(cache, cKey, cPath, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    return value
  }
  
  public func modifyJson(value: String, withPath: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cPath = withPath.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_modify_json(cache, cKey, cPath, cValue)
  }
  
  public func patchJson(patch: String, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    let cPatch = patch.cString(using: .utf8)
    return MemCache_patch_json(cache, cKey, cPatch)
  }
  
  deinit {
    MemCache_delete(cache)
  }
}
