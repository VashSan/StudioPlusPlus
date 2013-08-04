#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QUuid>
#include <QVector>
#include <QSharedPointer>

struct SourceFile;
struct ProjectDeclaration;

typedef QSharedPointer<ProjectDeclaration> ProjectPtr;

/*!
 * \brief Stores basic project specific information.
 */
struct ProjectDeclaration
{
	QString name;
	QString file;		/// The complete file path
	QUuid id;
	QUuid type;
	QList<SourceFile> sourceList;
};

/*!
 * \brief A SourceFile is a file belonging to a project.
 */
struct SourceFile
{
	/*!
	 * \brief Specifies the use of a SourceFile
	 */
	enum Type
	{
		Unknown = -1, /// We encountered an unknown item
		Unspecified,  /// A file which is not necessary for building
		CppHeader,    /// A C++ Header file
		CppSource     /// A C++ Source file
	};

	Type type;			/// The kind of the file
	QString file;		/// The file name only
	QString filePath;	/// The complete file path including file name
};

#endif // DATA_H
