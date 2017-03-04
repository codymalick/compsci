package main

import (
  "crypto/md5"
  "fmt"
  "io"
  "os"
  "log"
  "math/rand"
  "time"
)

const (
  dog="doge.jpg"
  cat="gcat.jpg"
  newCat="newGcat.jpg"
  newDog="newDoge.jpg"
)

func alterHashit(filename string, alteredFilename string) ([]byte, []byte) {
  file, err := os.Open(filename)
  if err != nil {
    log.Fatal(err)
  }

  defer file.Close()

  // Generate 20 bytes of random string
  newData := make([]byte, 20)
  rand.Read(newData)

  // copy cat file
  out, err := os.Create(alteredFilename)
  if err != nil { log.Fatal(err) }


  _, err = io.Copy(out, file)
  if err != nil { log.Fatal(err) }

  // Alter the file
  out.Write(newData)

  // close the file, otherwise weird behavior with md5
  out.Close()

  out2, err := os.Open(alteredFilename)
  defer out2.Close()
  // MD5 time
  hasher := md5.New()
  if _, err := io.Copy(hasher, out2); err != nil {
    panic(err)
  }

  return hasher.Sum(nil), newData

}

func hashit(filename string) ([]byte) {
  file, err := os.Open(filename)
  if err != nil {
    log.Fatal(err)
  }

  defer file.Close()

  hasher := md5.New()
  if _, err := io.Copy(hasher, file); err != nil {
    log.Fatal(err)
  }

  result := hasher.Sum(nil)

  return result
}

func recreateOutput(str string, filename,alteredFilename string) {
  file, err := os.Open(filename)
  if err != nil {
    log.Fatal(err)
  }

  defer file.Close()

  newData := []byte(str)

  // copy cat file
  out, err := os.Create(alteredFilename)
  if err != nil { log.Fatal(err) }


  _, err = io.Copy(out, file)
  if err != nil { log.Fatal(err) }

  // Alter the file
  out.Write(newData)

  // close the file, otherwise weird behavior with md5
  out.Close()

  out2, err := os.Open(alteredFilename)
  defer out2.Close()
  // MD5 time
  hasher := md5.New()
  if _, err := io.Copy(hasher, out2); err != nil {
    panic(err)
  }
}

func main() {
  rand.Seed(time.Now().UTC().UnixNano())

  // maps of all hashes seen
  dogHashList := make(map[string]string)
  catHashList := make(map[string]string)

  // Get the dog hash
  dogHash := hashit(dog)

  // Get cat hash
  catHash := hashit(cat)

  fmt.Printf("Dog: %x, Cat: %x\n", dogHash, catHash)

  dogHashList[" "] = string(dogHash)
  catHashList[" "] = string(catHash)

  for i := 0; i > -1; i++ {
     catHash, catString := alterHashit(cat, newCat)
     catComp := catHash[0:5]
     if val, ok := dogHashList[string(catComp)]; ok {
       fmt.Printf("Cat:string %x \ncat hash %x dog hash %x\n%v\n", catString, string(catComp), val,i)
       recreateOutput(val, dog, newDog)
       return
     }

     catHashList[string(catComp)] = string(catString)

     dogHash, dogString := alterHashit(dog, newDog)
     dogComp := dogHash[0:5]
     if val, ok := catHashList[string(dogComp)]; ok {
       fmt.Printf("Dog:string %x \ndog hash %x \ncat hash %x\n%v\n", dogString, string(dogComp), val,i)
       recreateOutput(val, cat, newCat)
       return
     }
     dogHashList[string(dogComp)] = string(dogString)
  }
}
