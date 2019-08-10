#pragma once
#include <string>
namespace dom {
	namespace error {
		typedef std::string DOMErrorType;
		namespace type {
			const DOMErrorType HierarchyRequestError = "HierarchyRequestError";
			const DOMErrorType WrongDocumentError = "WrongDocumentError";
			const DOMErrorType InvalidCharacterError = "InvalidCharacterError";
			const DOMErrorType NoModificationAllowedError = "NoModificationAllowedError";
			const DOMErrorType NotFoundError = "NotFoundError";
			const DOMErrorType NotSupportedError = "NotSupportedError";
			const DOMErrorType InUseAttributeError = "InUseAttributeError";
			const DOMErrorType InvalidStateError = "InvalidStateError";
			const DOMErrorType SyntaxError = "SyntaxError";
			const DOMErrorType InvalidModificationError = "InvalidModificationError";
			const DOMErrorType NamespaceError = "NamespaceError";
			const DOMErrorType SecurityError = "SecurityError";
			const DOMErrorType NetworkError = "NetworkError";
			const DOMErrorType AbortError = "AbortError";
			const DOMErrorType URLMismatchError = "URLMismatchError";
			const DOMErrorType QuotaExceededError = "QuotaExceededError";
			const DOMErrorType TimeoutError = "TimeoutError";
			const DOMErrorType InvalidNodeTypeError = "InvalidNodeTypeError";
			const DOMErrorType DataCloneError = "DataCloneError";
			const DOMErrorType EncodingError = "EncodingError";
			const DOMErrorType NotReadableError = "NotReadableError";
			const DOMErrorType UnknownError = "UnknownError";
			const DOMErrorType ConstraintError = "ConstraintError";
			const DOMErrorType DataError = "DataError";
			const DOMErrorType TransactionInactiveError = "TransactionInactiveError";
			const DOMErrorType ReadOnlyError = "ReadOnlyError";
			const DOMErrorType VersionError = "VersionError";
			const DOMErrorType OperationError = "OperationError";
			const DOMErrorType NotAllowedError = "NotAllowedError";
		}
	}
}