//
//  MemCache-Swift.swift
//  MemCacheDemo
//
//  Created by 叶永平 on 2023/4/24.
//

import Foundation

final public class MemCache {
  
  public init() {}
  
  public func getString(forKey key: String) -> String? {
    let cKey = key.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    
    let success = MemCache_get_string(cKey, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    cValue?.deallocate()
    return value
  }
  
  public func getInt32(forKey key: String) -> Int32? {
    let cKey = key.cString(using: .utf8)
    var value: Int32 = 0
    let success = MemCache_get_int(cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  public func getDouble(forKey key: String) -> Double? {
    let cKey = key.cString(using: .utf8)
    var value: Double = 0
    let success = MemCache_get_double(cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  public func getBool(forKey key: String) -> Bool? {
    let cKey = key.cString(using: .utf8)
    var value: Bool = true
    let success = MemCache_get_bool(cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  public func getData(forKey key: String) -> Data? {
    var size: size_t = 0
    var result: UnsafeMutablePointer<UInt8>?
    let successed = MemCache_get_bytes(key, &size, &result)
    guard let result = result, successed else {
      return nil
    }
    let data = Data(bytes: result, count: Int(size))
    result.deallocate()
    return data
  }
  
  public func delete(for key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_delete_value(cKey)
  }
  
  public func put(value: Int32, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_int(cKey, value)
  }
  
  public func put(value: Bool, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_bool(cKey, value)
  }
  
  public func put(value: Double, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_double(cKey, value)
  }
  
  public func put(value: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_put_string(cKey, cValue)
  }
  
  public func put(value: Data, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    let size = value.count
    return value.withUnsafeBytes { (bytes: UnsafeRawBufferPointer) -> Int32 in
      if let baseAddress = bytes.baseAddress {
        return MemCache_put_bytes(cKey, baseAddress.assumingMemoryBound(to: UInt8.self), size)
      } else {
        return -1
      }
    }
  }

  public func put(stringPairs: __owned [(String, String)]) throws -> Int32 {
    let keys = stringPairs.map { $0.0 }
    let values = stringPairs.map { $0.1 }
    let size = stringPairs.count
    return try keys.withCStringArray { keysBufferPointer in
      return try values.withCStringArray { valuesBufferPoint in
       return MemCache_put_strings(keysBufferPointer, valuesBufferPoint, size)
      }
    }
  }

  public func put(int32Pairs: __owned [(String, Int32)]) throws -> Int32 {
    let keys = int32Pairs.map { $0.0 }
    let values = int32Pairs.map { $0.1 }
    let size = int32Pairs.count
    return try keys.withCStringArray { keysBuffer in
      values.withUnsafeBufferPointer { valuesBuffer in
        MemCache_put_ints(keysBuffer, valuesBuffer.baseAddress, size)
      }
    }
  }
  
  public func put(doublePairs: __owned [(String, Double)]) throws -> Int32 {
    let keys = doublePairs.map { $0.0 }
    let values = doublePairs.map { $0.1 }
    let size = doublePairs.count
    return try keys.withCStringArray { keysBuffer in
      values.withUnsafeBufferPointer { valuesBuffer in
        MemCache_put_doubles(keysBuffer, valuesBuffer.baseAddress, size)
      }
    }
  }

  public func put(boolPairs: __owned [(String, Bool)]) throws -> Int32 {
    let keys = boolPairs.map { $0.0 }
    let values = boolPairs.map { $0.1 }
    let size = boolPairs.count
    return try keys.withCStringArray { keysBuffer in
      values.withUnsafeBufferPointer { valuesBuffer in
        MemCache_put_bools(keysBuffer, valuesBuffer.baseAddress, size)
      }
    }
  }
  
  public func deleteJson(for key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_delete_json(cKey)
  }
  
  public func putJson(value: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_put_json(cKey, cValue)
  }
  
  public func getJson(forkey key: String) -> String? {
    let cKey = key.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    
    let success = MemCache_get_json(cKey, &cValue)
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
    let success = MemCache_query_json(cKey, cPath, &cValue)
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
    return MemCache_modify_json(cKey, cPath, cValue)
  }
  
  public func patchJson(patch: String, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    let cPatch = patch.cString(using: .utf8)
    return MemCache_patch_json(cKey, cPatch)
  }
 
  public func tracing(for key: String, with type: MemCacheTracingOption) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_value_tracing(cKey, type.rawValue)
  }
  
  public func removeTracing(for key: String, with type: MemCacheTracingOption) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_remove_tracing(cKey, type.rawValue)
  }

}

enum CStringError: Error {
    case allocationFailed
}

extension Array where Element == String {
  public func withCStringArray<R>(
      _ body: (UnsafePointer<UnsafePointer<CChar>?>?) throws -> R
  ) throws -> R {
      var cStrings: [UnsafePointer<CChar>?] = []
      defer {
          cStrings.forEach {
              if let mutablePtr = UnsafeMutablePointer(mutating: $0) {
                  free(mutablePtr)
              }
          }
      }
      for arg in self {
          guard let cString = strdup(arg) else {
              throw CStringError.allocationFailed
          }
          cStrings.append(cString)
      }
      cStrings.append(nil)
      return try cStrings.withUnsafeBufferPointer { bufferPointer in
          try body(bufferPointer.baseAddress)
      }
  }
}

enum MemCacheValueType: Int32 {
  case intValue    =  1
  case doubleValue =  2
  case boolValue   =  3
  case stringValue =  4
  case dataValue   =  5
}

public struct MemCacheTracingOption: OptionSet {
  public let rawValue: Int32
  
  public init(rawValue: Int32) {
    self.rawValue = rawValue
  }

  public static let getOption = MemCacheTracingOption(rawValue: 1 << 4)
  public static let putOption = MemCacheTracingOption(rawValue: 1 << 5)
}

@inline(__always)
private func parseType(for type: Int32) -> (MemCacheValueType, MemCacheTracingOption)? {
  if type < (1 << 4) + 1 {
    return nil
  }
  let vt = type & ~((1 << 4) | (1 << 5))

  guard let valueType = MemCacheValueType(rawValue: vt) else {
    return nil
  }
  
  let tracingType = MemCacheTracingOption(rawValue: type & ((1 << 4) | (1 << 5)))
  return (valueType, tracingType)
}

@_cdecl("MemCache_getTracing")
public func getTracing(_ key: UnsafePointer<CChar>, _ value: UnsafeRawPointer, size: Int, type: Int32) {
  guard let (valueType, tracingType) = parseType(for: type) else { return }
  let _key = String(cString: key)
  switch valueType {
  case .stringValue:
    let _valuePtr = value.assumingMemoryBound(to: CChar.self)
    let _value = String(cString: _valuePtr)
    var tracing = "unknow"
    if tracingType.contains(.putOption) {
      tracing = "put"
    } else if tracingType.contains(.getOption) {
      tracing = "get"
    }
    print("\(tracing) \(_key) for string \(_value)")
    break
  case .doubleValue:
    let _valuePtr = value.assumingMemoryBound(to: Double.self)
    let _value = _valuePtr.pointee
    var tracing = "unknow"
    if tracingType.contains(.putOption) {
      tracing = "put"
    } else if tracingType.contains(.getOption) {
      tracing = "get"
    }
    print("\(tracing) \(_key) for double \(_value)")
    break
  case .intValue:
    let _valuePtr = value.assumingMemoryBound(to: Int32.self)
    let _value = _valuePtr.pointee
    var tracing = "unknow"
    if tracingType.contains(.putOption) {
      tracing = "put"
    } else if tracingType.contains(.getOption) {
      tracing = "get"
    }
    print("\(tracing) \(_key) for int32 \(_value)")
    break
  case .boolValue:
    let _valuePtr = value.assumingMemoryBound(to: Bool.self)
    let _value = _valuePtr.pointee
    var tracing = "unknow"
    if tracingType.contains(.putOption) {
      tracing = "put"
    } else if tracingType.contains(.getOption) {
      tracing = "get"
    }
    print("\(tracing) \(_key) for bool \(_value)")
    break
  case .dataValue:
    let data = Data(bytes: value, count: size)
    var tracing = "unknow"
    if tracingType.contains(.putOption) {
      tracing = "put"
    } else if tracingType.contains(.getOption) {
      tracing = "get"
    }
    if let s = String(data: data, encoding: .utf8) {
      print("\(tracing) \(_key) for data string \(s)")
    } else {
      print("\(tracing) \(_key) for data \(data)")
    }
    
    break
  }
}
