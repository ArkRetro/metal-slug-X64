/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x015520.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 candidate_15520(void)

{
  byte bVar3;
  uint uVar1;
  undefined2 uVar2;
  char cVar4;
  int unaff_A5;

  if (*(char *)(unaff_A5 + 0x42bf) == '\0') {
    if (thunk_FUN_000042f4[unaff_A5] == (code)0x0) {
      if ((((DAT_0010f000 & 2) == 0) && (*(char *)(unaff_A5 + 0x42bd) != '\0')) ||
         (bVar3 = candidate_15448(), bVar3 == 0)) {
        return 1;
      }
      if ((bVar3 & 2) == 0) {
        if ((bVar3 & 8) == 0) {
          return 1;
        }
        *(byte *)(unaff_A5 + 0x42bf) = *(byte *)(unaff_A5 + 0x42bf) | 2;
      }
      else {
        *(byte *)(unaff_A5 + 0x42bf) = *(byte *)(unaff_A5 + 0x42bf) | 1;
      }
    }
    else {
      *(code *)(unaff_A5 + 0x42bf) = thunk_FUN_000042f4[unaff_A5];
      thunk_FUN_000042f4[unaff_A5] = (code)0x0;
    }
    _DAT_0010c2c4 = 0;
    candidate_3caac();
    *(undefined1 *)(unaff_A5 + 0x42c2) = 0;
    *(code *)(unaff_A5 + 0x42ca) = FUN_0000435c[unaff_A5];
    *(undefined1 *)(unaff_A5 + 0x42cb) = *(undefined1 *)(unaff_A5 + 0x4362);
    if ((*(char *)(unaff_A5 + -0x7f78) != '\x02') || (DAT_0010c2e7 == '\0')) {
      uVar2 = 0;
      goto LAB_000156fa;
    }
    candidate_671fc();
    candidate_66f30();
    (&LAB_000042bc)[unaff_A5] = 0xff;
    *(undefined2 *)(thunk_FUN_0000449e + unaff_A5) = 0;
  }
  else if (DAT_0010c2bc == '\0') {
    uVar1 = candidate_15448();
    if ((*(byte *)(unaff_A5 + 0x42bf) & 1) == 0) {
LAB_00015652:
      if ((*(byte *)(unaff_A5 + 0x42bf) & 2) != 0) {
        if ((uVar1 & 8) != 0) goto LAB_000156e4;
        if (*(char *)(unaff_A5 + 0x42be) != '\0') {
          if ((*(byte *)(unaff_A5 + 0x4367) & 4) != 0) goto LAB_000156b8;
          if (((byte)candidate_4364[unaff_A5 + 2] & 4) != 0) goto LAB_000156c6;
        }
      }
      if (((*(byte *)(unaff_A5 + 0x42c2) & 0x18) == 0) &&
         ((*(char *)(unaff_A5 + 0x42c2) + 1U & 7) < 2)) {
        candidate_3d244();
      }
      uVar2 = 0;
      goto LAB_000156fa;
    }
    if ((uVar1 & 2) == 0) {
      if (*(char *)(unaff_A5 + 0x42be) == '\0') goto LAB_00015652;
      if ((*(byte *)(unaff_A5 + 0x4367) & 1) == 0) {
        if (((byte)candidate_4364[unaff_A5 + 2] & 1) == 0) goto LAB_00015652;
LAB_000156c6:
        cVar4 = *(char *)(unaff_A5 + 0x42c1);
        *(char *)(unaff_A5 + 0x42c1) = cVar4 + -1;
        if (cVar4 == '\0') {
          *(undefined1 *)(unaff_A5 + 0x42c1) = 1;
          uVar2 = 2;
        }
        else {
          uVar2 = 0;
        }
        goto LAB_000156fa;
      }
LAB_000156b8:
      *(undefined1 *)(unaff_A5 + 0x42c1) = 0xf;
      uVar2 = 2;
      goto LAB_000156fa;
    }
LAB_000156e4:
    if ((DAT_0010c2e7 != '\0') || (*(char *)(unaff_A5 + 0x42be) == '\0')) {
      uVar2 = 1;
      goto LAB_000156fa;
    }
    (&LAB_000042bc)[unaff_A5] = 0xff;
    candidate_671fc();
    candidate_67218();
    *(undefined2 *)(thunk_FUN_0000449e + unaff_A5) = 0;
  }
  cVar4 = candidate_672d0();
  if (cVar4 == '\0') {
    func_0x00009cce();
    if (((DAT_0010c2e7 == '\0') || (DAT_0010c30e == '\0')) || (_DAT_0010c30a != 0)) {
      uVar2 = 0;
    }
    else {
      uVar2 = 2;
    }
  }
  else {
    (&LAB_000042bc)[unaff_A5] = 0;
    if (*(code **)(thunk_FUN_0000435c + unaff_A5) != (code *)0x0) {
      (**(code **)(thunk_FUN_0000435c + unaff_A5))();
    }
    uVar2 = *(undefined2 *)(thunk_FUN_0000449e + unaff_A5);
    if ((char)uVar2 == '\x01') {
      func_0x0003cf12();
      uVar2 = 1;
    }
  }
LAB_000156fa:
  *(byte *)(unaff_A5 + 0x435d) =
       (byte)FUN_0000435c[unaff_A5] & ((byte)FUN_0000435c[unaff_A5] ^ *(byte *)(unaff_A5 + 0x42ca))
       | *(byte *)(unaff_A5 + 0x435d);
  *(byte *)(unaff_A5 + 0x4363) =
       *(byte *)(unaff_A5 + 0x4362) & (*(byte *)(unaff_A5 + 0x4362) ^ *(byte *)(unaff_A5 + 0x42cb))
       | *(byte *)(unaff_A5 + 0x4363);
  *(char *)(unaff_A5 + 0x42c2) = *(char *)(unaff_A5 + 0x42c2) + -1;
  if ((char)uVar2 == '\x01') {
    candidate_3d25a();
    (&LAB_000042bc)[unaff_A5] = 0;
    *(undefined1 *)(unaff_A5 + 0x42bf) = 0;
    func_0x000c8e5c();
    uVar2 = 1;
  }
  return uVar2;
}
